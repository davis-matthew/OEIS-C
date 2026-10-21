/*
 * A208170  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 62*a(n-1) - 333*a(n-2) + 396*a(n-3) - 135*a(n-4) for n>5
 *            with a(1)=5, a(2)=159, a(3)=8469, a(4)=473928, a(5)=26625693
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1 */

    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)159;
    if (n == 3) return (seq_t)8469;
    if (n == 4) return (seq_t)473928;
    if (n == 5) return (seq_t)26625693;

    seq_t v2 = (seq_t)159;          /* a(2) */
    seq_t v3 = (seq_t)8469;         /* a(3) */
    seq_t v4 = (seq_t)473928;       /* a(4) */
    seq_t v5 = (seq_t)26625693;     /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t newv = (seq_t)62 * v5 - (seq_t)333 * v4 + (seq_t)396 * v3 - (seq_t)135 * v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = newv;
    }
    return v5;
}
