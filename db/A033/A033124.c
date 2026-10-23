/*
 * A033124  Base-6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A033124: Base‑6 digits formed by the periodic pattern 1,0,1.
 * Recurrence (n ≥ 5):
 *     a(n) = 6*a(n-1) + a(n-3) – 6*a(n-4)
 * with initial values
 *     a(1) = 1,  a(2) = 6,  a(3) = 37,  a(4) = 223.
 * The function memoises computed values so that calls may be made
 * in arbitrary order.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for out‑of‑range indices; for n ≥ 1 the function is defined. */
    if (n == 0) return (seq_t)0;

    /* Static storage for memoisation. */
    static seq_t *cache       = NULL;   /* array indexed from 1 */
    static idx_t cache_size   = 0;      /* number of allocated slots (≥ idx_t(n+1)) */
    static idx_t max_computed = 0;      /* highest index that has a valid value in cache */

    /* Ensure we can store indices up to n and the four base values. */
    idx_t need = (n < 4) ? 5 : n + 1; /* at least indices 1‑4 */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)need * sizeof(seq_t));
    } else if ((size_t)need > (size_t)cache_size) {
        cache = (seq_t *)realloc(cache, (size_t)need * sizeof(seq_t));
    }
    cache_size = need;

    /* Initialise the four base terms (they are valid for all n). */
    if (n >= 1) cache[1] = (seq_t)1;
    if (n >= 2) cache[2] = (seq_t)6;
    if (n >= 3) cache[3] = (seq_t)37;
    if (n >= 4) cache[4] = (seq_t)223;

    /* After setting the base terms, we consider indices up to 4 as computed. */
    if (max_computed < 4) max_computed = 4;

    /* Compute any missing terms up to n using the recurrence. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1]
                     +          cache[i - 3]
                     - (seq_t)6 * cache[i - 4];
        }
        max_computed = n;
    }

    return cache[n];
}
