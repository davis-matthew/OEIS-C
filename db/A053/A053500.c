/*
 * A053500  Number of degree-n permutations of order dividing 10.
 * https://oeis.org/A053500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{l∈{1,2,5,10}} binom(n‑1, l‑1)·(l‑1)!·a(n‑l)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"

static seq_t *dp_cache = NULL;      /* cache of computed values          */
static size_t dp_cap   = 0;         /* current allocated capacity        */
static idx_t  dp_len   = 0;         /* number of elements stored         */

/* Cycle lengths whose permutations have order dividing 10   */
static const idx_t cycle_len[4]  = {1, 2, 5, 10};
static const seq_t fact_val[4]   = { (seq_t)1, (seq_t)1, (seq_t)24, (seq_t)362880 };

/* Ensure dp_cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= dp_cap)
        return;

    size_t new_cap = dp_cap ? dp_cap : 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                     /* double until large enough */

    seq_t *new_buf = (seq_t *)realloc(dp_cache, new_cap * sizeof(seq_t));
    if (!new_buf)                           /* allocation failure   */
        return;

    dp_cache = new_buf;
    dp_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (!dp_cache) {
        dp_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!dp_cache)                     /* allocation failure   */
            return (seq_t)0;
        dp_cap = 2;
        dp_len = 0;
    }

    if ((size_t)(n + 1) > dp_cap)
        ensure_capacity(n);

    /* Compute any missing values up to n */
    if (n >= dp_len) {
        /* Base case */
        if (dp_len == 0) {
            dp_cache[0] = (seq_t)1;
            dp_len = 1;
        }

        for (idx_t k = dp_len; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (int i = 0; i < 4; ++i) {
                idx_t l = cycle_len[i];
                if (l > k)
                    continue;

                seq_t comb  = binom(k - 1, l - 1);    /* C(k-1, l-1) */
                seq_t term  = comb * fact_val[i];     /* * (l-1)!  */
                term       *= dp_cache[k - l];        /* * a(k-l)  */
                sum        += term;
            }
            dp_cache[k] = sum;
        }
        dp_len = n + 1;
    }

    return dp_cache[n];
}
