/*
 * A052736  E.g.f. [1 -3x -sqrt(1-6x+x^2) -x*(1-x-sqrt(1-6x+x^2)) ]/2.
 * https://oeis.org/A052736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies * a(1)=0, a(2)=2, a(3)=24, * for n≥1: *
 *            (-n³‑n²+4n+4)·a(n) + (-6+7n²+11n)·a(n+1) + (-7n‑10)·a(n+2) +
 *            a(n+3) = 0 * Thus a(n+3) = (n³+n²‑4n‑4)·a(n) +
 *            (6‑7n²‑11n)·a(n+1) + (7n+10)·a(n+2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise with first four terms */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);          /* allocation failure */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 24;
    }

    if (n < cache_len) return cache[n];

    /* extend the cache up to n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);          /* allocation failure */

        for (idx_t k = cache_len; k <= n; ++k) {
            idx_t m = k - 3;                    /* current n in recurrence */
            long long nl = (long long)m;         /* signed integer for coeffs */

            long long coeff1   = nl*nl*nl + nl*nl - 4*nl - 4;      /* may be negative */
            long long coeff3   = 7*nl + 10;                       /* positive */
            long long coeff2abs = 7*nl*nl + 11*nl - 6;            /* positive */

            __int128 term1 = (__int128)cache[m]   * (__int128)coeff1;
            __int128 term3 = (__int128)cache[m+2] * (__int128)coeff3;
            __int128 term2 = (__int128)cache[m+1] * (__int128)coeff2abs;

            __int128 val = term1 + term3 - term2;   /* should be non‑negative */
            cache[k] = (seq_t)val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
