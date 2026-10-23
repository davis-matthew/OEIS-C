/*
 * A015096  Carlitz-Riordan q-Catalan numbers (recurrence version) for q=11.
 * https://oeis.org/A015096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n+1) = Σ_{i=0}^{n} 11^i * a(i) * a(n-i) (q = 11)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of previously computed sequence values.  The array grows
   as needed and is reordered when a larger n is requested.  All
   indices and sizes are stored in native C types; the arithmetic
   uses the seq_t type supplied by the build. */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;

/* Return the nth Carlitz‑Riordan q‑Catalan number for q = 11.
   The function is memoised and can handle calls in arbitrary order. */
seq_t a(idx_t n)
{
    /* Ensure we have a cache big enough for 0 … n.  If the existing
       cache is too small, realloc it and compute the missing values. */
    if ((size_t)cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        seq_cache = tmp;

        size_t old_size = cache_size;
        cache_size = new_size;

        /* Initialise a(0) if this is the first allocation. */
        if (old_size == 0) {
            seq_cache[0] = (seq_t)1;
            old_size = 1;
        }

        /* Compute all missing terms a(k) with old_size ≤ k ≤ n.  */
        const seq_t q = (seq_t)11;          /* the constant q = 11 */
        for (size_t k = old_size; k <= (size_t)n; ++k) {
            seq_t total = 0;
            seq_t pow   = 1;                /* 11^i for current i */
            for (size_t i = 0; i <= k - 1; ++i) {
                seq_t term = pow * seq_cache[i] * seq_cache[k - 1 - i];
                total += term;
                pow <<= 0;                   /* placeholder to avoid warnings */
                pow *= q;                    /* next power of 11 */
            }
            seq_cache[k] = total;
        }
    }

    return seq_cache[(size_t)n];
}
