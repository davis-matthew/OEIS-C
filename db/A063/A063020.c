/*
 * A063020  Reversion of y - y^2 - y^3 + y^4.
 * https://oeis.org/A063020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies 16*(n-1)*n*(2*n-1)*(17*n-27)*a(n) =
 *            (n-1)*(1819*n^3-6527*n^2+7350*n-2520)*a(n-1) +
 *            8*(2*n-3)*(4*n-9)*(4*n-7)*(17*n-10)*a(n-2), with a(0)=0, a(1)=1,
 *            a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 32, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n) {
    /* cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t max_computed = 0;/* highest index already computed */

    /* Ensure that the cache can hold indices up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp);
        cache = tmp;
        cache_cap = new_cap;
        /* initialise newly allocated area to zero */
        for (idx_t i = max_computed + 1; i < cache_cap; ++i) cache[i] = (seq_t)0;
    }

    /* initialise base values if needed */
    if (max_computed < 0) max_computed = 0;   /* safety for first call */
    if (n >= 0 && cache_cap > 0) cache[0] = (seq_t)0;
    if (n >= 1 && cache_cap > 1) cache[1] = (seq_t)1;
    if (n >= 2 && cache_cap > 2) cache[2] = (seq_t)1;

    /* compute missing entries up to n */
    idx_t start = max_computed + 1;
    if (start < 3) start = 3;  /* recurrence starts at n=3 */

    for (idx_t k = start; k <= n; ++k) {
        /* use 128‑bit intermediates when seq_t fits in 8 bytes */
        if (sizeof(seq_t) <= 8) {
            __int128 left  = (__int128)16 * (k-1) * k * (2*k-1) * (17*k-27);

            __int128 A = (__int128)(k-1) *
                         ( (__int128)1819 * k * k * k
                         - (__int128)6527 * k * k
                         + (__int128)7350 * k
                         - 2520 );

            __int128 term1 = A * (__int128)cache[k-1];

            __int128 B = (__int128)8 * (2*k-3) * (4*k-9) * (4*k-7) * (17*k-10);
            __int128 term2 = B * (__int128)cache[k-2];

            __int128 sum = term1 + term2;
            cache[k] = (seq_t)(sum / left);
        } else {
            /* arbitrary‑precision arithmetic */
            seq_t left = 16;
            left *= (k-1);
            left *= k;
            left *= (2*k-1);
            left *= (17*k-27);

            seq_t A = (k-1);
            seq_t tmp = (seq_t)1819 * ( (seq_t)k * k * k );
            tmp -= (seq_t)6527 * ( (seq_t)k * k );
            tmp += (seq_t)7350 * k;
            tmp -= 2520;
            A *= tmp;

            seq_t term1 = A * cache[k-1];

            seq_t B = 8;
            B *= (2*k-3);
            B *= (4*k-9);
            B *= (4*k-7);
            B *= (17*k-10);

            seq_t term2 = B * cache[k-2];

            seq_t sum = term1 + term2;
            cache[k] = sum / left;
        }
    }

    if (n > max_computed) max_computed = n;
    return cache[n];
}
