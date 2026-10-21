/*
 * A095685  Expansion of (1+x)^4/(1-11*x+11*x^2-x^3).
 * https://oeis.org/A095685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0)=1, a(1)=15, a(2)=160, a(3)=1600, a(4)=15856
 * For n≥5 : a(n) = 11*a(n‑1) – 11*a(n‑2) + a(n‑3)
 *
 * This follows from the generating function
 *   G(x) = (1+x)^4 / (1 – 11x + 11x^2 – x^3).
 * The recurrence is linear with constant integer coefficients,
 * so it can be computed memoised in O(n) time.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;   /* number of elements currently stored */

    /* Allocate at least n+1 entries, but keep a minimum of 5 to hold the
     * first five terms that are needed for the recurrence. */
    size_t alloc_sz = (size_t)(n + 1);
    if (alloc_sz < 5) alloc_sz = 5;

    seq_t *tmp = (seq_t *)realloc(memo, alloc_sz * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failed – return 0 as a fallback */
    memo = tmp;

    /* First time: initialise the first five terms. */
    if (memo_size == 0) {
        memo[0] = 1;
        memo[1] = 15;
        memo[2] = 160;
        memo[3] = 1600;
        memo[4] = 15856;
        memo_size = 5;
    }

    /* If we already have a value for this index, just return it. */
    if (n < memo_size)
        return memo[n];

    /* Compute any missing values up to n using the recurrence. */
    idx_t start = memo_size;
    if (start < 5) start = 5;    /* recurrence valid only from n = 5 onward */

    for (idx_t k = start; k <= n; ++k) {
        memo[k] = 11 * memo[k-1] - 11 * memo[k-2] + memo[k-3];
    }

    memo_size = alloc_sz;
    return memo[n];
}
