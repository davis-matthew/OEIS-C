/*
 * A093637  G.f.: A(x) = Product_{n>=0} 1/(1 - a(n)*x^(n+1)) = Sum_{n>=0}
 *          a(n)*x^n.
 * https://oeis.org/A093637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n≥1, a(n) = Σ_{partitions of n} ∏_{p∈partition}
 *            a(p‑1) equivalently, a is the Euler transform of the shifted
 *            sequence (a(k‑1))ₖ≥1. The product form is A(x) = ∏_{k≥1} 1/(1 –
 *            a(k‑1)·x^k)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised cache for the sequence up to the largest requested index. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Return a(n) for the sequence A093637.  The function is memoised so
   repeated calls are fast; it will recompute the entire table if a
   larger index than ever requested is needed.  All arithmetic is done
   using seq_t operands, which may be an unsigned integer type or an
   arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* If necessary, grow the cache and recompute everything up to n. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)calloc((size_t)new_size, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failed */

        /* a(0) = 1.  All other entries start at 0. */
        new_cache[0] = (seq_t)1;

        /* Compute a(1) .. a(n) using the recurrence derived from the
           product form:   A(x) = ∏_{k≥1} 1/(1 – b_k·x^k),    b_k = a(k‑1).  */
        for (idx_t k = 1; k < new_size; ++k) {
            seq_t bk = new_cache[k - 1];          /* b_k = a(k-1) */
            for (idx_t i = k; i < new_size; ++i) {
                new_cache[i] += new_cache[i - k] * bk;
            }
        }

        /* Replace old cache. */
        if (cache) free(cache);
        cache     = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
