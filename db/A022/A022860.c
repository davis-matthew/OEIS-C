/*
 * A022860  a(n) = [ a(n-1)/a(1) ] + [ a(n-3)/a(3) ] + [ a(n-5)/a(5) ] + ...,
 *          for n >= 3.
 * https://oeis.org/A022860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A022860
 *
 *  a(1) = 1, a(2) = 1, a(3) = 1
 *  a(n) = ⌊a(n-1)/a(1)⌋ + ⌊a(n-3)/a(3)⌋ + ⌊a(n-5)/a(5)⌋ + ...   (n ≥ 4)
 *  (sum over odd j with 1 ≤ j ≤ n-1, using integer division)
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, calloc */

/*  Cached array of computed terms.
 *  cache[0] is unused so that a(1) is at cache[1].                  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;         /* allocated size (includes index 0)   */
static idx_t  computed_up_to = 0;    /* highest index that has been filled */

/*  Ensure that cache contains values up to n (inclusive).        */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the array if necessary.  Use calloc to zero new slots. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;   /* include index 0 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    } else if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        /* zero the newly added part */
        for (size_t i = old_cap; i < cache_cap; ++i) {
            cache[i] = (seq_t)0;
        }
    }

    /* Initialise the first three known terms.  We do this each time
       in case the array was freshly allocated or re‑expanded.     */
    if (cache_cap > 1) cache[1] = (seq_t)1;
    if (cache_cap > 2) cache[2] = (seq_t)1;
    if (cache_cap > 3) cache[3] = (seq_t)1;
    computed_up_to = (cache_cap > 3) ? 3 : (idx_t)(cache_cap - 1);

    /* Compute any missing values up to n.                         */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[(size_t)i] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j < i; j += 2) {           /* odd j: 1,3,5,… */
            sum += cache[(size_t)(i - j)] / cache[(size_t)j];
        }
        cache[(size_t)i] = sum;
    }
    computed_up_to = n;
}

/*  Public function required by the build.  Computes a(n).        */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for non‑positive indices */
    ensure_cache(n);
    return cache[(size_t)n];
}
