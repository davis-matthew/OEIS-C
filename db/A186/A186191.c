/*
 * A186191  First digit of tetranacci numbers A000288.
 * https://oeis.org/A186191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A186191 – first digit of the tetranacci numbers  
   (T(0)=T(1)=T(2)=T(3)=1, T(n)=T(n-1)+T(n-2)+T(n-3)+T(n-4))  
   The first digit is obtained from the fractional part of log10(T(n)).  */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* -- cache for logarithms and first digits --------------------------------- */
    static long double *log_t = NULL;          /* log10 of T(i)              */
    static uint8_t    *first   = NULL;          /* first decimal digit (1‑9)  */
    static idx_t      cache_sz = 0;             /* size of allocated arrays   */

    /* initialise caches on first call */
    if (cache_sz == 0) {
        cache_sz = 4;                            /* we know values for i=0..3   */
        log_t    = (long double *)malloc(cache_sz * sizeof(long double));
        first    = (uint8_t *)malloc(cache_sz * sizeof(uint8_t));
        if (!log_t || !first) return (seq_t)0;   /* allocation failed          */

        /* initial tetranacci numbers are 1 → log10 = 0, first digit = 1 */
        for (idx_t i = 0; i < 4; ++i) {
            log_t[i] = 0.0L;
            first[i] = 1;
        }
    }

    /* grow caches if n is outside current bounds */
    if (n >= cache_sz) {
        idx_t new_sz = n + 1;
        long double *tmp_log = (long double *)realloc(log_t, new_sz * sizeof(long double));
        uint8_t    *tmp_first = (uint8_t *)realloc(first, new_sz * sizeof(uint8_t));
        if (!tmp_log || !tmp_first) return (seq_t)0; /* allocation failed          */
        log_t = tmp_log;
        first = tmp_first;

        /* compute values for indices cache_sz .. n */
        for (idx_t i = cache_sz; i <= n; ++i) {
            /* compute log10( T(i-1)+T(i-2)+T(i-3)+T(i-4) )
               by using the log‑sum‑exp trick */
            long double m = log_t[i-1];
            if (log_t[i-2] > m) m = log_t[i-2];
            if (log_t[i-3] > m) m = log_t[i-3];
            if (log_t[i-4] > m) m = log_t[i-4];

            long double sum = 0.0L;
            sum += powl(10.0L, log_t[i-1] - m);
            sum += powl(10.0L, log_t[i-2] - m);
            sum += powl(10.0L, log_t[i-3] - m);
            sum += powl(10.0L, log_t[i-4] - m);

            log_t[i] = m + log10l(sum);

            /* fractional part gives leading digit */
            long double frac   = log_t[i] - floorl(log_t[i]);
            long double leading = powl(10.0L, frac);
            uint8_t digit = (uint8_t)(leading + 1e-12L);   /* guard against 8.9999… */
            if (digit == 0) digit = 1;                    /* safety for rounding */
            first[i] = digit;
        }
        cache_sz = new_sz;
    }

    return (seq_t)first[n];
}
