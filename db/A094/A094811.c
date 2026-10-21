/*
 * A094811  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 8 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 6.
 * https://oeis.org/A094811
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094811
   a(n) = 6*a(n-1) - 10*a(n-2) + 4*a(n-3) (n ≥ 5)
   a(2) = 1, a(3) = 6, a(4) = 26
   Defined for n ≥ 2, values below the offset are returned as 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Values below the offset are undefined – return 0. */
    if (n < 2) return (seq_t)0;

    /* Static memoisation buffer. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* Ensure the buffer can hold the requested index. */
    if (memo_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        memo = tmp;
        for (idx_t i = memo_size; i < new_size; ++i) memo[i] = (seq_t)0;
        memo_size = new_size;
    }

    /* Compute missing terms only. */
    for (idx_t i = 2; i <= n; ++i) {
        if (memo[i] != 0) continue;          /* already computed */

        if (i == 2) {
            memo[i] = (seq_t)1;
        } else if (i == 3) {
            memo[i] = (seq_t)6;
        } else if (i == 4) {
            memo[i] = (seq_t)26;
        } else {
            /* Use (6*a_{i-1} + 4*a_{i-3}) - 10*a_{i-2} to avoid underflow. */
            seq_t part1 = (seq_t)6 * memo[i-1];
            seq_t part3 = (seq_t)4 * memo[i-3];
            seq_t part2 = (seq_t)10 * memo[i-2];
            memo[i] = (part1 + part3) - part2;
        }
    }

    return memo[n];
}
