/*
 * A074000  Number of elements of GF(3^n) with trace 0 and subtrace 0.
 * https://oeis.org/A074000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*a(n-2) + 9*a(n-3) + 18*a(n-4) + 27*a(n-5) with initial
 *            values a(1)=1, a(2)=1, a(3)=3, a(4)=9, a(5)=21 for n ≥ 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Store a(1)..a(5) and independent terms in a static memo array. */
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;          /* number of allocated slots */
    static const seq_t init[6] = {0, 1, 1, 3, 9, 21};

    /* Direct access for the first five terms. */
    if (n <= 5)
        return init[n];

    /* Allocate/resize the memo array if needed. */
    if (n + 1 > memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (memo) {
            memcpy(new_memo, memo, memo_size * sizeof(seq_t));
            free(memo);
        }
        memo = new_memo;
        memo_size = new_size;

        /* initialise the first five values, if they are not already set */
        for (idx_t i = 1; i <= 5; ++i)
            memo[i] = init[i];
    }

    /* Compute missing terms up to n using the recurrence.  We start at 6. */
    for (idx_t i = 6; i <= n; ++i) {
        memo[i] = (seq_t)3 * memo[i - 2]
                + (seq_t)9 * memo[i - 3]
                + (seq_t)18 * memo[i - 4]
                + (seq_t)27 * memo[i - 5];
    }

    return memo[n];
}
