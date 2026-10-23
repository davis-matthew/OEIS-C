/*
 * A040731  Continued fraction for sqrt(759).
 * https://oeis.org/A040731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(759).  a(0) = 27, 
   thereafter the periodic part 1,1,4,1,1,54 repeats.  This implementation
   generates the period once using integer arithmetic and reuses it for all
   calls. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Integer part of √759 */
    static seq_t a0 = 0;          /* stored as seq_t for return */
    static seq_t period[20];      /* periodic part of the continued fraction */
    static size_t period_len = 0;
    static int initialized = 0;

    if (!initialized) {
        /* compute floor(sqrt(759)) without floating point */
        idx_t v = 0;
        /* use 128‑bit intermediate to avoid overflow on small values */
        while (((unsigned long long)(v + 1) * (v + 1)) <= 759ULL)
            ++v;
        a0 = (seq_t)v;
        idx_t a0v = (idx_t)a0;   /* integer representation for internal use */

        /* generate the periodic part of the continued fraction for √759 */
        const idx_t D = 759ULL;
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0v;   /* a0 as integer */

        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0v + m) / d;
            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0v);   /* period ends when a == 2*a0 */

        initialized = 1;
    }

    if (n == 0)
        return a0;

    size_t idx = (size_t)((n - 1) % period_len);
    return period[idx];
}
