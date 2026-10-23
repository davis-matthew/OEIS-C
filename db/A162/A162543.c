/*
 * A162543  A Chebyshev transform of the large Schroeder numbers A006318.
 * https://oeis.org/A162543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   (n+1)*a(n) = (5-n)*a(n-6) + 3*(2*n-7)*a(n-5) + (11-4*n)*a(n-4) +
 *            12*(n-2)*a(n-3) + (5-4*n)*a(n-2) + 3*(2*n-1)*a(n-1),
 *            a(0)=1,a(1)=2,a(2)=5,a(3)=18,a(4)=73,a(5)=312
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  computed = (idx_t)-1; /* none computed yet */

    /* we need at least 6 entries to hold the base values */
    size_t needed = n + 1;
    if (needed < 6) needed = 6;

    if (cache == NULL || cap < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cap   = needed;
    }

    /* initialize base values on first call */
    if (computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)73;
        cache[5] = (seq_t)312;
        computed = 5;
    }

    /* compute all values up to n if needed */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            /* recurrence only for i >= 6 (base values already added) */
            if (i < 6) {
                cache[i] = (seq_t)i; /* unreachable but safe */
                continue;
            }

#ifndef __cplusplus
            /* C build – use 128‑bit arithmetic to avoid overflow */
            __int128 rhs = 0;
            rhs += (__int128)cache[i-5] * (3 * (2*i-7));
            rhs += (__int128)cache[i-3] * (12 * (i-2));
            rhs += (__int128)cache[i-1] * (3 * (2*i-1));
            rhs -= (__int128)cache[i-6] * (i-5);
            rhs -= (__int128)cache[i-4] * (4*i-11);
            rhs -= (__int128)cache[i-2] * (4*i-5);
            cache[i] = (seq_t)(rhs / (i+1));
#else
            /* C++ build – seq_t is arbitrary‑precision */
            seq_t rhs = 0;
            rhs += cache[i-5] * (seq_t)(3 * (2*i-7));
            rhs += cache[i-3] * (seq_t)(12 * (i-2));
            rhs += cache[i-1] * (seq_t)(3 * (2*i-1));
            rhs -= cache[i-6] * (seq_t)(i-5);
            rhs -= cache[i-4] * (seq_t)(4*i-11);
            rhs -= cache[i-2] * (seq_t)(4*i-5);
            cache[i] = rhs / (i+1);
#endif
        }
        computed = n;
    }
    return cache[n];
}
