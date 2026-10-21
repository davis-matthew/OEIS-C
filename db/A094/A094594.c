/*
 * A094594  Total number of edges in all connected labeled graphs on n nodes.
 * https://oeis.org/A094594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n,2)*2^(binomial(n,2)-1) - Sum_{k=1}^{n-1}
 *            binomial(n,k)*2^binomial(n-k,2)*a(k) Initial value: a(1) = 0
 *            (Recurrence implemented with memoisation for fast repeated
 *            calls.)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Cache already computed values */
    static seq_t *cache      = NULL;   /* indices 0 .. current_size-1 */
    static idx_t current_size = 0;     /* allocated size (>=1) */
    static idx_t highest_computed = 0;

    if (n <= 1)
        return 0;          /* a(1) = 0 (also for n=0 which is unused) */

    /* Grow the cache array to accommodate index n */
    if (n >= current_size) {
        size_t new_size = (size_t)n + 1;          /* include index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort();                             /* allocation failure */

        /* Initialise new slots to zero */
        for (size_t i = current_size; i < new_size; ++i)
            new_cache[i] = 0;

        cache        = new_cache;
        current_size = (idx_t)new_size;
    }

    /* Compute values up to n that are not yet cached */
    for (idx_t m = highest_computed + 1; m <= n; ++m) {

        if (m <= 1) {          /* a(1) or a(0) is zero */
            cache[m] = 0;
            continue;
        }

        /* binomial(m,2) as a standard integer (fits in idx_t for n<=~10000) */
        idx_t edges = (m * (m - 1)) / 2;

        /* 2^(edges-1) */
        seq_t pow_half = pow2(edges - 1);

        seq_t term1 = (seq_t)edges * pow_half;     /* binomial(m,2) * 2^(edges-1) */

        seq_t sum = 0;

        for (idx_t k = 1; k < m; ++k) {
            seq_t binom_nk = binom(m, k);          /* C(m,k) as seq_t */

            idx_t edges_ck = ((m - k) * (m - k - 1)) / 2;   /* C(m-k,2) as idx_t */
            seq_t pow_ck   = pow2(edges_ck);          /* 2^C(m-k,2) */

            seq_t term = binom_nk * pow_ck * cache[k];
            sum += term;
        }

        cache[m] = term1 - sum;
    }

    highest_computed = n;
    return cache[n];
}
