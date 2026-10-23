/*
 * A112091  Number of idempotent order-preserving partial transformations (of
 *          an n-element chain).
 * https://oeis.org/A112091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 5*(a(n-1) - a(n-2)), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;   /* number of values currently stored */

    /* initialise for the first call or if memo is empty */
    if (memo_len == 0) {
        memo_len = 2;
        memo = (seq_t*)malloc(sizeof(seq_t) * memo_len);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
    }

    /* extend memoisation if necessary */
    if (n >= memo_len) {
        idx_t new_len = n + 1;
        memo = (seq_t*)realloc(memo, sizeof(seq_t) * new_len);

        /* compute missing values */
        for (idx_t i = memo_len; i <= n; ++i) {
            seq_t diff = memo[i - 1] - memo[i - 2];
            memo[i] = (seq_t)5 * diff + (seq_t)1;
        }

        memo_len = new_len;
    }

    return memo[n];
}
