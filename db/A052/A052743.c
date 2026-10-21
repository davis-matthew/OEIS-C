/*
 * A052743  E.g.f. ( 1-x-sqrt(1-2*x+x^2-4*x^3) )/(2*x^2).
 * https://oeis.org/A052743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Recurrence a(1) = 1, a(2) = 2, a(3) = 6, a(0) = 0 For n ≥ 0 (
 *            -56*n^2-26*n^3-12-46*n-4*n^4 )*a(n) + ( n^3+7*n^2+16*n+12
 *            )*a(n+1) + ( -13*n-2*n^2-21 )*a(n+2) + ( n+5 )*a(n+3) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* early return for small n requiring no cache */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)0;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)6;
        }
    }

    /* ensure cache has enough space to hold index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialize base values once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache_len = 4;
    }

    /* compute missing terms up to n */
    while (cache_len <= n) {
        idx_t idx = cache_len;          /* index of term to compute (>=4) */
        unsigned __int128 iv = (unsigned __int128)(idx - 3);   /* n in recurrence */

        /* compute polynomial coefficients in 128‑bit */
        unsigned __int128 p0 = 4*iv*iv*iv*iv + 26*iv*iv*iv + 56*iv*iv + 46*iv + 12;
        unsigned __int128 p2 = 13*iv + 2*iv*iv + 21;
        unsigned __int128 coeff1 = iv*iv*iv + 7*iv*iv + 16*iv + 12;
        unsigned __int128 den = iv + 5;

        /* fetch previous terms */
        seq_t a_n   = cache[idx - 3];
        seq_t a_np1 = cache[idx - 2];
        seq_t a_np2 = cache[idx - 1];

        /* compute numerator = p0*a_n + p2*a_np2 - coeff1*a_np1 */
        unsigned __int128 term1 = p0 * (unsigned __int128)a_n;
        unsigned __int128 term2 = p2 * (unsigned __int128)a_np2;
        unsigned __int128 term3 = coeff1 * (unsigned __int128)a_np1;

        unsigned __int128 numerator = term1 + term2 - term3;

        seq_t next = (seq_t)(numerator / den);
        cache[idx] = next;
        cache_len++;
    }

    return cache[n];
}
