/*
 * A027151  a(n) = T(n,0) + T(n,1) + ... + T(n,n), T given by A027144.
 * https://oeis.org/A027151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) + 3*a(n-4) (n>3); a(0)=1, a(1)=3, a(2)=7,
 *            a(3)=21
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization table */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;

    /* The table must hold indices 0 .. n inclusive, and at least 0..3 for
       the initial values.  Compute the needed size.                     */
    idx_t required = n + 1;
    if (required < 4)
        required = 4;

    /* First allocation */
    if (!memo) {
        memo = (seq_t*)malloc((size_t)required * sizeof(seq_t));
        if (!memo) /* allocation failure – return zero (undefined in tests) */
            return (seq_t)0;

        /* Initial known terms */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)3;
        memo[2] = (seq_t)7;
        memo[3] = (seq_t)21;

        /* Compute any further terms up to the required size */
        for (idx_t i = 4; i < required; ++i) {
            memo[i] = (seq_t)(6 * memo[i - 2] + 3 * memo[i - 4]);
        }
        memo_size = required;
    }
    /* Expand table if the current size is insufficient */
    else if (memo_size < required) {
        seq_t *tmp = (seq_t*)realloc(memo, (size_t)required * sizeof(seq_t));
        if (!tmp) /* allocation failure – return zero */
            return (seq_t)0;
        memo = tmp;

        for (idx_t i = memo_size; i < required; ++i) {
            memo[i] = (seq_t)(6 * memo[i - 2] + 3 * memo[i - 4]);
        }
        memo_size = required;
    }

    return memo[n];
}
