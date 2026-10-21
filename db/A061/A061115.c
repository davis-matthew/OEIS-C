/*
 * A061115  Concatenation of numbers in n-th row of triangle of unsigned
 *          Stirling numbers of first kind (A008275).
 * https://oeis.org/A061115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concat_{k=1..n} s(n,k) where s(n,k) is the unsigned
 *            Stirling number of the first kind (A008275) and the
 *            concatenation is in decimal, with no separators
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Fast path for indices below the defined offset */
    if (n == 0)
        return (seq_t)0;

    /* Static state for memoisation */
    static idx_t computed_max = 0;        /* highest n whose row is computed */
    static seq_t *stirling = NULL;       /* triangular array of Stirling numbers */
    static size_t stirling_cap = 0;      /* capacity of the stirling array (>= n+1) */
    static seq_t *cache = NULL;          /* cache of concatenations for n=1.. */
    static size_t cache_cap = 0;         /* capacity of cache (>= n) */

    /* Grow the Stirling array if necessary */
    if (stirling_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(stirling, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0) /* allocation failed */
            exit(EXIT_FAILURE);
        stirling = tmp;
        stirling_cap = new_cap;
    }

    /* Grow the cache if necessary */
    if (cache_cap < n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the base of the Stirling triangle once */
    if (computed_max == 0)
        stirling[0] = (seq_t)1;

    /* Compute rows from computed_max+1 up to n */
    for (idx_t cur = computed_max + 1; cur <= n; ++cur) {
        /* Ensure the current element exists; initialise to 0 */
        stirling[cur] = (seq_t)0;

        /* Update the current row in reverse to avoid overwriting values
           that are still needed for the recurrence. */
        for (idx_t k = cur; k > 0; --k) {
            stirling[k] = stirling[k - 1] + (seq_t)(cur - 1) * stirling[k];
        }
        /* The Stirling number s(cur,0) is always 0 for cur >= 1 */
        stirling[0] = (seq_t)0;

        /* Concatenate the numbers of this row */
        seq_t ans = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t v = stirling[k];
            int d = digit_len(v);                 /* decimal digits of v */
            seq_t mult = pow10((idx_t)d);         /* 10^d */
            ans = ans * mult + v;
        }
        cache[cur - 1] = ans;   /* 1‑based index stored at 0‑based position */
    }

    computed_max = n;
    return cache[n - 1];
}
