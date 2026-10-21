/*
 * A056016  a(n) = -2*a(n - 1) -a(n - 2) -a(n - 3), a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A056016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n - 1) - a(n - 2) - a(n - 3), a(0) = a(1) = a(2) = 1
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 3) {
        return (seq_t)1;
    }

    /* memoisation storage */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static size_t memo_len = 0;   /* next index that needs computation */

    /* initialise or grow storage */
    if (memo == NULL) {
        memo_cap = (size_t)n + 1;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = memo[1] = memo[2] = (seq_t)1;
        memo_len = 3;
    } else if ((size_t)n >= memo_cap) {
        size_t new_cap = memo_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        memo = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        memo_cap = new_cap;
    }

    /* compute all missing terms up to n */
    if ((size_t)n >= memo_len) {
        for (size_t i = memo_len; i <= (size_t)n; ++i) {
            seq_t a1 = memo[i - 1];
            seq_t a2 = memo[i - 2];
            seq_t a3 = memo[i - 3];
            seq_t val = (seq_t)(-2) * a1 - a2 - a3;
            memo[i] = val;
        }
        memo_len = (size_t)n + 1;
    }

    return memo[(size_t)n];
}
