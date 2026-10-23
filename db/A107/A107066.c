/*
 * A107066  Expansion of 1/(1-2*x+x^5).
 * https://oeis.org/A107066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5), a(0) = 1, a(1) = 2, a(2) = 4, a(3) =
 *            8, a(4) = 16
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;

    /* Handle indices below the offset */
    if (n < 5) {
        static const seq_t base[5] = { 1, 2, 4, 8, 16 };
        return base[n];
    }

    /* Initialise memoisation array with the first five terms */
    if (memo == NULL) {
        memo = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!memo) return 0;          /* minimal error handling */
        memo[0] = 1; memo[1] = 2; memo[2] = 4;
        memo[3] = 8; memo[4] = 16;
        memo_len = 5;
    }

    /* Extend the table up to the requested index */
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* minimal error handling */
        memo = tmp;
        for (idx_t i = memo_len; i < new_len; ++i) {
            /* a(i) = 2*a(i-1) - a(i-5) */
            memo[i] = 2 * memo[i-1] - memo[i-5];
        }
        memo_len = new_len;
    }

    return memo[n];
}
