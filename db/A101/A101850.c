/*
 * A101850  A Catalan transform of Pell(n+1).
 * https://oeis.org/A101850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+3) = (4*(4*n+9)*a(n+2) - (31*n+45)*a(n+1) - 2*(2*n+3)*a(n))
 *            / (2*(n+3)) initial values: a(0)=1, a(1)=2, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: this implementation uses a recurrence that was proved to be exact
   (see the OEIS entry). For seq_t types of width <= 64 bits the
   intermediate arithmetic is performed in signed 128‑bit so that no
   overflow occurs during the numerator computation. For larger seq_t
   types the natural arithmetic of the type is used. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of allocated array   */
    static size_t cache_len = 0;   /* number of valid entries in cache */

    /* initialise cache with base values  a(0)=1, a(1)=2, a(2)=7 */
    if (cache_len == 0) {
        cache_cap = 4;                           /* start with small space */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;              /* allocation error */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache_len = 3;
    }

    /* return cached value if already known */
    if (n < cache_len)
        return cache[n];

    /* grow cache up to required index */
    for (size_t m = cache_len; m <= n; ++m) {
        seq_t result;

        if (m < 3) {              /* base terms already set */
            result = cache[m];
        } else {
            /* indices needed for recurrence */
            seq_t a_m2 = cache[m - 1];   /* a(m-1) = a(n+2)   */
            seq_t a_m1 = cache[m - 2];   /* a(m-2) = a(n+1)   */
            seq_t a_m0 = cache[m - 3];   /* a(m-3) = a(n)     */
            idx_t  nm3   = (idx_t)(m - 3); /* n = m-3          */
            idx_t  mm    = (idx_t)m;      /* m   = n+3         */

            /* compute using 128‑bit arithmetic when seq_t is 64‑bit or below */
            if (sizeof(seq_t) <= 8) {
                __int128 num =  (__int128)4 * ((__int128)4*nm3 + 9) * (__int128)a_m2
                               - (__int128)(31*nm3+45) * (__int128)a_m1
                               - (__int128)2 * ((__int128)2*nm3 + 3) * (__int128)a_m0;
                __int128 den = (__int128)2 * mm;
                result = (seq_t)(num / den);
            } else {
                /* seq_t is large (e.g., 128‑bit or arbitrary‑precision) */
                seq_t num = (__int128)4 * (((__int128)4*nm3) + 9) * a_m2;
                seq_t term2 = (__int128)(31*nm3+45) * a_m1;
                seq_t term3 = (__int128)2 * (((__int128)2*nm3)+3) * a_m0;
                num = num - term2;
                num = num - term3;
                seq_t den = (seq_t)2 * (seq_t)m;
                result = num / den;  /* exact division */
            }
        }

        /* ensure sufficient capacity */
        if (m >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 4;
            while (new_cap <= m) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                cache_cap = cache_len = 0;
                return (seq_t)0;  /* allocation error */
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[m] = result;
        cache_len = m + 1;
    }

    return cache[n];
}
