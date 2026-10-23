/*
 * A176785  Sequence with e.g.f. g(x) = -(1/2)*sqrt(2*exp(-2*x)-1) + 1/2.
 * https://oeis.org/A176785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1)^{n+1} * 2^{n-1} + Sum_{k=1}^{n-1} binom(n,k) * a(k)
 *            * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

/* External linkage: sequence function a(idx_t n) */
seq_t a(idx_t n)
{
    /* Static cache to make repeated calls efficient */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;   /* size of allocated array          */
    static idx_t   cache_max = 0;   /* highest index computed so far    */

    /* Ensure cache is large enough to hold a[n] */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        cache_len = 1;                /* we store a[0] only so far   */
        cache[0] = (seq_t)0;           /* a(0) = 0                     */
        cache_max = 0;
    }
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure as fallback */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute values up to n if not already computed */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum over k = 1 .. i-1 of binom(i,k) * a[k] * a[i-k] */
        for (idx_t k = 1; k < i; ++k) {
            seq_t bin   = binom((idx_t)i, (idx_t)k);
            seq_t prod  = cache[(size_t)k] * cache[(size_t)(i - k)];
            prod = prod * bin;
            sum += prod;
        }
        /* term = 2^{i-1} with sign depending on parity of i */
        seq_t term = pow2((idx_t)(i - 1));
        if ((i & 1) == 0) {          /* i even -> (-1)^{i+1} = -1 */
            cache[(size_t)i] = sum - term;   /* guaranteed non‑negative        */
        } else {                     /* i odd -> (+1)                     */
            cache[(size_t)i] = sum + term;
        }
    }
    cache_max = n;
    return cache[(size_t)n];
}
