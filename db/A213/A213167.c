/*
 * A213167  a(n) = n! - (n-2)!.
 * https://oeis.org/A213167
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! - (n-2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoized factorial cache */
static seq_t *fact_cache = NULL;
static size_t fact_cache_cap = 0;

/* Ensure factorials up to n are computed */
static void ensure_factorial_cache(idx_t n)
{
    /* If we already have enough, nothing to do */
    if ((size_t)n < fact_cache_cap)
        return;

    size_t old_cap = fact_cache_cap;          /* previous capacity       */
    size_t new_cap = old_cap > 0 ? old_cap : 4;/* grow factor            */

    /* Find a new capacity that exceeds n */
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    /* Reallocate the cache */
    fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));

    /* Initialise factorial[0] and factorial[1] if this is the first call */
    if (old_cap == 0) {
        fact_cache[0] = (seq_t)1;
        fact_cache[1] = (seq_t)1;
        old_cap = 2;
    }

    /* Compute missing factorial values up to new_cap-1 */
    for (size_t i = old_cap; i < new_cap; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)(i);

    fact_cache_cap = new_cap;
}

/* a(n) = n! - (n-2)!  for n >= 2 */
seq_t a(idx_t n)
{
    /* The sequence is only defined for n >= 2; behaviour below is undefined */
    ensure_factorial_cache(n);
    return fact_cache[(size_t)n] - fact_cache[(size_t)(n - 2)];
}
