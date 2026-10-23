/*
 * A042097  Denominators of continued fraction convergents to sqrt(572).
 * https://oeis.org/A042097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 574*a(n-4) - a(n-8) for n > 7
 * Initial values: a(0)=1, a(1)=1, a(2)=11, a(3)=12,
 *                 a(4)=563, a(5)=575, a(6)=6313, a(7)=6888
 */

#include <stddef.h>
#include <stdint.h>

/* Maximum n expected for this implementation.  The specification
 * requires fast execution for n up to about 10 000.  The array
 * capacity is therefore fixed at 10 001 elements.  For larger n a
 * different strategy would be needed. */
#define CACHE_CAP 10001

/* Static cache for memoisation.  Index 0..CACHE_CAP-1. */
static seq_t cache[CACHE_CAP] = {0};

/* Highest index that has been computed and stored in cache.  It starts
 * at (idx_t)-1 which indicates that nothing has been computed yet. */
static idx_t max_computed = (idx_t)-1;

/* Helper array of the first eight values.  The values are cast to
 * seq_t; if seq_t cannot hold the actual value it will be truncated,
 * which is acceptable because seq_t may be a smaller integer type. */
static const seq_t init_vals[8] = {
    (seq_t)1,
    (seq_t)1,
    (seq_t)11,
    (seq_t)12,
    (seq_t)563,
    (seq_t)575,
    (seq_t)6313,
    (seq_t)6888
};

/* Compute a(n) using memoisation and the recurrence relation. */
seq_t a(idx_t n)
{
    /* If the value is already cached, return it immediately. */
    if (n < CACHE_CAP && cache[(size_t)n] != (seq_t)0)
        return cache[(size_t)n];

    /* If n exceeds the cache capacity, be conservative and compute
     * only up to CACHE_CAP-1.  In practice this should not happen
     * for the required n-range. */
    if (n >= CACHE_CAP)
        n = CACHE_CAP - 1;

    /* Compute values from the next missing index up to n.  The loop
     * runs in order, so all required previous terms are already
     * available when they are needed. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i < 8)
        {
            cache[(size_t)i] = init_vals[(size_t)i];
        }
        else
        {
            cache[(size_t)i] = (seq_t)574 * cache[(size_t)(i - 4)] - cache[(size_t)(i - 8)];
        }
    }

    max_computed = n;
    return cache[(size_t)n];
}
