/*
 * A128209  Jacobsthal numbers(A001045) + 1.
 * https://oeis.org/A128209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + (2^n - (-1)^n)/3 (equivalently: a(0)=1, a(1)=2,
 *            a(2)=2, a(n)=2*a(n-1)+a(n-2)-2*a(n-3) for n>=3)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 1,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamically sized cache */
static idx_t computed_upto = 0;    /* number of indices already computed */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for index n */
    if (n + 1 > computed_upto) {
        /* Reallocate cache to hold indices 0..n */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)(n + 1));
        /* If realloc fails, cache may be NULL – we let the program crash
           as this situation indicates out‑of‑memory.  */
        computed_upto = n + 1;
    }

    /* Compute values from the first missing index up to n */
    for (idx_t i = 0; i < computed_upto; ++i) {
        /* Values 0,1,2 are set below; the loop below will handle them too. */
    }

    /* Fill in the base cases if necessary */
    if (n >= 0 && cache[0] == 0) cache[0] = 1; /* a(0)=1 */
    if (n >= 1 && cache[1] == 0) cache[1] = 2; /* a(1)=2 */
    if (n >= 2 && cache[2] == 0) cache[2] = 2; /* a(2)=2 */

    /* Compute remaining terms using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        /* No need to check if cache[i] already set – the loop only runs
           for indices that were not computed yet. */
        cache[i] = 2 * cache[i - 1] + cache[i - 2] - 2 * cache[i - 3];
    }

    /* Update the count of computed elements */
    if (computed_upto <= n + 1)
        computed_upto = n + 1;

    return cache[n];
}
