/*
 * A109731  a(n) = - 4*a(n-2) - a(n-4), a(0) = 1, a(1) = -4, a(2) = -6, a(3) =
 *          15.
 * https://oeis.org/A109731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-2) - a(n-4) (a(0)=1, a(1)=-4, a(2)=-6, a(3)=15)
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *memo = NULL;
    static size_t memo_size = 0;

    /* Initialise base values once */
    if (memo == NULL) {
        memo_size = 4;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        if (memo == NULL)
            return (seq_t)0;               /* allocation failed – return 0 */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)-4;
        memo[2] = (seq_t)-6;
        memo[3] = (seq_t)15;
    }

    /* Fast return for already known indices */
    if (n < memo_size)
        return memo[(size_t)n];

    /* Expand the table to contain all indices up to n */
    size_t required = (size_t)n + 1;
    if (required > memo_size) {
        seq_t *tmp = (seq_t *)realloc(memo, required * sizeof(seq_t));
        if (tmp == NULL)
            return memo[(size_t)n];   /* allocation failed – return the last known value */
        memo = tmp;
        for (size_t i = memo_size; i <= (size_t)n; ++i) {
            /* i >= 4 here, so memo[i-2] and memo[i-4] are valid */
            seq_t val = -(seq_t)4 * memo[i-2] - memo[i-4];
            memo[i] = val;
        }
        memo_size = required;
    }

    return memo[(size_t)n];
}
