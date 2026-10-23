/*
 * A191006  Number of n X n symmetric binary matrices with each 1 adjacent to
 *          exactly 1 diagonally neighboring 1.
 * https://oeis.org/A191006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=1..n} sum_{i=0..⌊(k+1)/3⌋} binomial(k+1-2i, i)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = a(n-1) * S(n) where S(n) = Σ_{i=0..⌊(n+1)/3⌋} binomial(n+1-2i, i) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* cache[n-1] holds a(n) */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */

/* Compute a(n) for n >= 1, using memoised recurrence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* defined only for n >= 1 */

    /* initialise cache for a(1) if empty */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;       /* a(1) = 1 */
        cache_len = 1;
    }

    /* extend cache up to requested n */
    if ((idx_t)cache_len < n) {
        /* double capacity until it holds n terms */
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute new terms */
        for (size_t k = cache_len; k < (size_t)n; ++k) {
            idx_t kk = (idx_t)k + 1;   /* k is 0-based index for a(k+1) */
            idx_t i_max = (kk + 1) / 3;
            seq_t fk = (seq_t)0;
            for (size_t i = 0; i <= (size_t)i_max; ++i) {
                fk += binomial((idx_t)(kk + 1 - 2 * (idx_t)i),
                               (idx_t)i);
            }
            cache[k] = cache[k - 1] * fk;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
