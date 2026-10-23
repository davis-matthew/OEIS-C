/*
 * A192653  Binomial convolution of the Floor-Sqrt transform of Catalan
 *          numbers.
 * https://oeis.org/A192653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k) * floor_sqrt(catalan(k)) *
 *            floor_sqrt(catalan(n-k))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/floor_sqrt.h"

/* cache of floor_sqrt(catalan(i)) for i≤current_max_n */
static seq_t *sqrtcat_cache = NULL;
static idx_t sqrtcat_size  = 0;

/* ensure cache has entries up to n (inclusive) */
static void ensure_sqrtcat(idx_t n)
{
    if (n < sqrtcat_size) return;            /* already cached */

    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
    if (!tmp) return;                        /* allocation failed: treat as no further values */

    /* copy existing values */
    for (idx_t i = 0; i < sqrtcat_size; ++i)
        tmp[i] = sqrtcat_cache[i];

    /* compute missing entries */
    for (idx_t i = sqrtcat_size; i < new_size; ++i)
        tmp[i] = floor_sqrt(catalan(i));

    /* free old cache and install new one */
    if (sqrtcat_cache != NULL)
        free(sqrtcat_cache);
    sqrtcat_cache = tmp;
    sqrtcat_size  = new_size;
}

seq_t a(idx_t n)
{
    ensure_sqrtcat(n);
    seq_t result = 0;
    seq_t binom  = 1;          /* binomial(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * sqrtcat_cache[k] * sqrtcat_cache[n - k];
        result += term;

        if (k < n)
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }
    return result;
}
