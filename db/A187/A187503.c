/*
 * A187503  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-1,0,1,2}, n=3*r+p_i, and define a(-1)=1. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,1,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^3-2*x) with x=2*cos(Pi/9).
 * https://oeis.org/A187503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 3*a(n-6) - a(n-9) - a(n-12) for n >= 12
 *            Initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,
 *            a(5)=1,a(6)=1,a(7)=1,a(8)=1,a(9)=2,a(10)=3,a(11)=4
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   ensure_allocated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_allocated.h"

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static seq_t *memo = NULL;
    static size_t memo_cap = 0;
    static idx_t memo_size = 0;

    /* Immediate return if already computed */
    if (n < memo_size)
        return memo[n];

    /* Make sure the array is large enough to hold n+1 elements */
    if ((idx_t)(n + 1) > memo_cap)
        ensure_allocated(n + 1, &memo_cap, (void **)&memo, sizeof(seq_t));

    /* Initialise the first 12 values if not already done */
    if (memo_size < 12) {
        if (memo_cap < 12)
            ensure_allocated(12, &memo_cap, (void **)&memo, sizeof(seq_t));

        for (idx_t i = memo_size; i < 12; ++i) {
            if (i <= 4)
                memo[i] = 0;
            else if (i <= 8)
                memo[i] = 1;
            else if (i == 9)
                memo[i] = 2;
            else if (i == 10)
                memo[i] = 3;
            else /* i == 11 */
                memo[i] = 4;
        }
        memo_size = 12;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = memo_size; i <= n; ++i) {
        memo[i] = 2 * memo[i - 3] + 3 * memo[i - 6]
                  - memo[i - 9] - memo[i - 12];
    }
    memo_size = n + 1;

    return memo[n];
}
