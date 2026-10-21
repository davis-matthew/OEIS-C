/*
 * A089928  a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4), with a(0)=1, a(1)=2, a(3)=4,
 *          a(4)=10.
 * https://oeis.org/A089928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) + a(n-4); a(0)=1, a(1)=2, a(2)=4,
 *            a(3)=10
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t size  = 0;

    /* initialise first four terms */
    if (size == 0) {
        size = 4;
        memo = (seq_t *)malloc(size * sizeof(seq_t));
        if (!memo) return (seq_t)0;          /* out of memory – unlikely for the test harness */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)4;
        memo[3] = (seq_t)10;
    }

    if (n < size)
        return memo[n];

    for (idx_t i = size; i <= n; ++i) {
        if (i == size) {
            /* enlarge buffer to hold up to term n */
            seq_t *newmemo = (seq_t *)malloc((n + 1) * sizeof(seq_t));
            memcpy(newmemo, memo, size * sizeof(seq_t));
            free(memo);
            memo = newmemo;
            size = n + 1;
        }
        /* recurrence: a(i) = 2*a(i-1) + 2*a(i-3) + a(i-4) */
        memo[i] = (memo[i-1] << 1) + (memo[i-3] << 1) + memo[i-4];
    }

    return memo[n];
}
