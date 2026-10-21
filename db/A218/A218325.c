/*
 * A218325  Even heptagonal pyramidal numbers.
 * https://oeis.org/A218325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) + 320 (for n ≥ 10) Initial
 *            values (n = 1 … 9): 8, 26, 60, 196, 308, 456, 880, 1166, 1508
 *            The recurrence uses only small integer multiplications and
 *            additions, so it can be computed for arbitrary‑precision seq_t
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 1 … 9  – index 0 kept as dummy */
    static const seq_t init[10] = {
        0,       /* dummy for 0 */
        8, 26, 60, 196, 308, 456, 880, 1166, 1508
    };

    /* Cache for memoisation */
    static seq_t *cache      = NULL;   /* array of computed values */
    static size_t cache_cap  = 0;      /* allocated size (elements) */
    static size_t cache_max  = 0;      /* highest index that has been filled */

    /* Base case: small n – no recurrence needed */
    if (n <= 9)
        return init[(size_t)n];

    /* Make sure we have enough storage for up to index `n`. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – caller will ignore */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 9 entries if not yet done */
    if (cache_max < 9) {
        for (size_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        cache_max = 9;
    }

    /* Compute missing entries up to n */
    if (cache_max < (size_t)n) {
        for (size_t i = cache_max + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)3 * cache[i - 3]
                       - (seq_t)3 * cache[i - 6]
                       + cache[i - 9]
                       + (seq_t)320;
        }
        cache_max = (size_t)n;
    }

    return cache[n];
}
