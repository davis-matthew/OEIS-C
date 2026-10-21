/*
 * A083952  Integer coefficients a(n) of A(x), where a(n) = 1 or 2 for all n,
 *          such that A(x)^(1/2) has only integer coefficients.
 * https://oeis.org/A083952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A083952:
 *   b(0) = 1
 *   For n ≥ 1:
 *     S_n = Σ_{i=1}^{n-1} b(i)·b(n−i)
 *     a(n) = 1  if S_n is odd
 *            2  if S_n is even
 *     b(n) = (a(n) − S_n) / 2          (exact integer)
 * The sequence a(n) ∈ {1,2} is uniquely defined and satisfies that
 * the square root of the generating function has integer coefficients. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External linkage as required */
seq_t a(idx_t n) {
    /* static caches for already computed terms */
    static seq_t *a_cache = NULL;
    static int64_t *b_cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* grow cache to at least n+1 terms */
    if (n >= cache_cap || cache_cap == 0) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        b_cache = (int64_t *)realloc(b_cache, new_cap * sizeof(int64_t));
        cache_cap = new_cap;
    }

    /* initialise base case if needed */
    if (cache_len == 0) {
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        b_cache[0] = (int64_t)1; /* b(0) = 1 */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* compute S_i = sum_{k=1}^{i-1} b(k) * b(i-k) */
        __int128 S = 0;
        for (idx_t k = 1; k < i; ++k) {
            S += (__int128)b_cache[k] * (__int128)b_cache[i - k];
        }
        /* determine a(i) */
        seq_t ai = (S & 1) ? (seq_t)1 : (seq_t)2;
        a_cache[i] = ai;
        /* compute b(i) = (a(i) - S) / 2 */
        __int128 diff = (__int128)ai - S; /* even */
        int64_t bi = (int64_t)(diff >> 1); /* /2 */
        b_cache[i] = bi;
    }
    cache_len = n + 1;
    return a_cache[n];
}
