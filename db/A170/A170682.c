/*
 * A170682  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: A170682
 *
 * a(0) = 1
 * For n ≥ 1:
 *   let p(n) = 1           if n == 0 or n == 49
 *   p(n) = 2               if 1 ≤ n ≤ 48
 *   p(n) = 0               otherwise
 *   a(n) = p(n) + 47 * Σ_{k=1}^{min(48,n)} a(n-k) - 1128 * a(n-49)
 *     (subtraction term is omitted when n < 49).
 *
 * The following implementation uses a static cache and extends it
 * lazily.  It runs in O(n) time per new value requested and works
 * for any size of seq_t, including arbitrary‑precision integers.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache   = NULL;   /* array of seq_t values      */
    static size_t cache_len = 0;    /* number of valid entries    */
    static size_t cache_cap = 0;    /* allocated capacity         */

    /* extend cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory → zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if n already computed, return it */
    if (n < cache_len) return cache[(size_t)n];

    /* compute new terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t p;
        if (i == 0 || i == 49)        p = (seq_t)1;
        else if (i <= 48)             p = (seq_t)2;
        else                          p = (seq_t)0;

        /* sum of the last up to 48 preceding terms */
        seq_t sum48 = (seq_t)0;
        size_t limit = (i >= 48) ? 48 : i;
        for (size_t k = 1; k <= limit; ++k)
            sum48 += cache[i - k];

        seq_t term = p + (seq_t)47 * sum48;

        if (i >= 49)
            term -= (seq_t)1128 * cache[i - 49];

        cache[i] = term;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
