/*
 * A213406  G.f.: exp( Sum_{n>=1} binomial(12*n-1, 6*n) * x^n/n ).
 * https://oeis.org/A213406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A213406
   G.f.: A(x) = exp( Sum_{n>=1} binomial(12*n-1, 6*n) * x^n / n )
   Recurrence (derived from the exponential generating function):
     a(0) = 1
     a(n) = (1/n) * Σ_{k=1}^{n} a(n-k) * binomial(12*k-1, 6*k)   for n ≥ 1
*/

#include <stddef.h>                         /* size_t */
#include "../utils/binomial.h"              /* binomial(idx_t a, idx_t b) */
#include "../utils/seq_realloc.h"           /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

/* Return the n-th term of sequence A213406 (0‑based index).  */
seq_t a(idx_t n)
{
    /* Cache for a[0 … last_computed] */
    static seq_t *a_cache   = NULL;
    static size_t a_cap     = 0;        /* current allocation size of a_cache */
    static idx_t  a_max_idx = 0;        /* highest index computed in a_cache */

    /* Cache for b[k] = binomial(12*k-1, 6*k), k ≥ 1 */
    static seq_t *b_cache   = NULL;
    static size_t b_cap     = 0;        /* current allocation size of b_cache */
    static idx_t  b_max_idx = 0;        /* highest index computed in b_cache */

    /* a(0) is the first term */
    if (n == 0)
        return (seq_t)1;

    /* Grow a_cache if necessary.  Allocate at least n+1 entries. */
    if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        a_cache = seq_realloc(a_cache, a_cap, new_cap);
        a_cap   = new_cap;
    }

    /* Ensure a_cache[0] is set.  (It stays 1 once set.) */
    if (a_max_idx == 0 && a_cache != NULL)
        a_cache[0] = (seq_t)1;

    /* Grow b_cache if necessary.  Allocate at least n+1 entries. */
    if (b_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        b_cache = seq_realloc(b_cache, b_cap, new_cap);
        b_cap   = new_cap;
    }

    /* Compute a[i] for all indices i that have not yet been computed. */
    for (idx_t i = a_max_idx + 1; i <= n; ++i) {
        /* Compute b[i] = binomial(12*i-1, 6*i) if not already present. */
        if (b_max_idx < i) {
            /* binomial accepts idx_t values; 12*i-1 and 6*i are well within
               idx_t limits for all n we are asked to support (n ≤ 10000).   */
            b_cache[i] = binomial((idx_t)(12 * i - 1), (idx_t)(6 * i));
            b_max_idx = i;
        }

        /* Compute the convolution sum for a[i]. */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += a_cache[i - k] * b_cache[k];
        }

        /* Divide by i (exact division). */
        a_cache[i] = sum / (seq_t)i;
    }

    /* Update the maximum computed index. */
    a_max_idx = n;

    return a_cache[n];
}
