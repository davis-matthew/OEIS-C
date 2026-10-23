/*
 * A195067  G.f. A(x) satisfies A(x) = Sum{n>=0} x^n * A(2*n*x).
 * https://oeis.org/A195067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} 2^k * (n-k)^k * a(k) for n>0, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>               /* malloc, realloc, NULL */
#include "../utils/big_pow.h"     /* big_pow(seq_t, seq_t) */

seq_t a(idx_t n) {
    /* Static cache for previously computed values. */
    static seq_t *cache   = NULL;   /* a(0) .. a(cache_len-1) */
    static idx_t  cache_len = 0;    /* number of allocated slots */

    /* Static table of powers of two: pow2[i] = 2^i. */
    static seq_t *pow2   = NULL;
    static idx_t  pow2_len = 0;     /* highest index stored in pow2 */

    /* ---------- Ensure cache has space for n ---------- */
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)1;          /* allocation failure */
        for (idx_t i = 0; i < cache_len; ++i) cache[i] = (seq_t)0;
        cache[0] = (seq_t)1;                         /* a(0) = 1 */
    } else if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)1;          /* allocation failure */
        for (idx_t i = old_len; i < cache_len; ++i) cache[i] = (seq_t)0;
    }

    /* ---------- Ensure pow2 has space for n ---------- */
    if (pow2 == NULL) {
        pow2_len = n + 1;
        pow2 = (seq_t *)malloc(pow2_len * sizeof(seq_t));
        if (pow2 == NULL) return (seq_t)1;          /* allocation failure */
        for (idx_t i = 0; i < pow2_len; ++i) pow2[i] = (seq_t)0;
        pow2[0] = (seq_t)1;                         /* 2^0 = 1 */
    } else if (pow2_len <= n) {
        idx_t old_len = pow2_len;
        pow2_len = n + 1;
        pow2 = (seq_t *)realloc(pow2, pow2_len * sizeof(seq_t));
        if (pow2 == NULL) return (seq_t)1;          /* allocation failure */
        for (idx_t i = old_len; i < pow2_len; ++i) pow2[i] = (seq_t)0;
        /* Compute newly needed powers */
        for (idx_t i = old_len; i < pow2_len; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;
    }

    /* ---------- Compute all missing a(m) up to n ---------- */
    for (idx_t m = 1; m <= n; ++m) {
        if (cache[m] != (seq_t)0) continue;        /* already computed */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t term = cache[k];                 /* a(k) */
            term *= pow2[k];                       /* * 2^k */
            term *= big_pow((seq_t)(m - k), (seq_t)k); /* * (m-k)^k */
            sum += term;
        }
        cache[m] = sum;
    }

    return cache[n];
}
