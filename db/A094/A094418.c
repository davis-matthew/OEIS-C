/*
 * A094418  Generalized ordered Bell numbers Bo(5,n).
 * https://oeis.org/A094418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A094418: generalized ordered Bell numbers Bo(5,n)
   Recurrence: a(0) = 1
               a(n) = 5 * Σ_{k=1..n} binomial(n,k) * a(n-k)   for n ≥ 1
*/

#include <stdlib.h>

/* Seq and index types are supplied by the build system */
static seq_t *cache = NULL;   /* dynamic cache of computed terms */
static size_t cache_size = 0; /* current cache size (number of slots) */

/* Expand cache to hold at least indices 0..n */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= cache_size)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!tmp)
        abort();                           /* allocation failure */

    /* Initialize newly allocated slots to 0 (they will be overwritten) */
    for (size_t i = cache_size; i < required; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_size = required;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                         /* a(0) = 1 */

    /* remember old size before we possibly grow the cache */
    size_t old_size = cache_size;
    ensure_cache(n);

    /* compute any terms that are still zero */
    for (size_t i = old_size; i <= (size_t)n; ++i)
    {
        if (i == 0) {                    /* base case, should already be 1 */
            cache[0] = 1;
            continue;
        }

        seq_t sum = 0;
        seq_t binom = (seq_t)i;           /* binomial (i,1) */

        for (idx_t k = 1; k <= i; ++k)
        {
            sum += binom * cache[i - k];

            /* update binom to C(i, k+1) */
            if (k < i)
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }

        cache[i] = sum * (seq_t)5;        /* a(i) = 5 * sum */
    }

    return cache[(size_t)n];
}
