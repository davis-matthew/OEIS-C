/*
 * A102900  a(n) = 3*a(n-1) + 4*a(n-2), a(0)=a(1)=1.
 * https://oeis.org/A102900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *memo = NULL;   /* array of computed values   */
    static idx_t memo_len = 0;   /* number of elements stored  */
    static idx_t memo_cap = 0;   /* allocated capacity (size)  */

    /* Ensure storage for indices 0..n (i.e., n+1 elements) */
    if (memo_cap <= n) {
        idx_t new_cap = (memo_cap == 0) ? 2 : memo_cap;
        while (new_cap <= n) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(memo, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;                   /* allocation failure */
        memo = tmp;
        memo_cap = new_cap;
    }

    /* Initialise base cases if not yet done */
    if (memo_len == 0) {
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo_len = 2;
    }

    /* Return cached value if already computed */
    if (n < memo_len) {
        return memo[n];
    }

    /* Compute missing terms up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        memo[i] = (seq_t)3 * memo[i - 1] + (seq_t)4 * memo[i - 2];
    }

    memo_len = n + 1;
    return memo[n];
}
