/*
 * A049221  Number of horizontally convex n-ominoes in which the top row has
 *          exactly 1 square, which is not above the rightmost square in the
 *          second row.
 * https://oeis.org/A049221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) initial values: a(1)=1,
 *            a(2)=0, a(3)=1, a(4)=4, a(5)=14 (recurrence valid for n>=6)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    if (n == 5) return (seq_t)14;

    /* Recurrence for n >= 6 */
    seq_t prev3 = (seq_t)1;   /* a(3) */
    seq_t prev2 = (seq_t)4;   /* a(4) */
    seq_t prev1 = (seq_t)14;  /* a(5) */
    seq_t curr;

    for (idx_t i = 6; i <= n; ++i) {
        /* temp = 5*a(n-1) + 4*a(n-3) ensures the subtraction never underflows for unsigned types */
        seq_t temp = (seq_t)5 * prev1 + (seq_t)4 * prev3;
        curr = temp - (seq_t)7 * prev2;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
