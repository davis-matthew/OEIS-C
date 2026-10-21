/*
 * A081034  7th binomial transform of the periodic sequence (1,8,1,1,8,1...).
 * https://oeis.org/A081034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 14*a(n-1) - 48*a(n-2),  a(0)=1, a(1)=15. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;      /* number of entries already stored */
    static idx_t memo_cap = 0;      /* allocated capacity */

    /* Ensure the memo array is large enough for indices up to n */
    idx_t required = n + 1;          /* size needed (indices 0..n) */

    /* Initialise on first call */
    if (memo == NULL) {
        memo_cap = 16;
        while (memo_cap < required) memo_cap <<= 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) exit(1);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)15;
        memo_len = 2;
    }

    /* Grow the array if necessary and compute missing entries */
    if ((idx_t)memo_len < required) {
        while (memo_cap < required) memo_cap <<= 1;
        memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
        if (!memo) exit(1);

        for (idx_t i = memo_len; i <= n; ++i) {
            if (i == 0) {       /* already set, but keep for completeness */
                memo[0] = (seq_t)1;
            } else if (i == 1) {
                memo[1] = (seq_t)15;
            } else {
                memo[i] = (seq_t)14 * memo[i-1] - (seq_t)48 * memo[i-2];
            }
        }
        memo_len = required;
    }

    return memo[n];
}
