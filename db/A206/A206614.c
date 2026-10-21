/*
 * A206614  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having no more than two equal edges, and new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A206614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + 36*a(n-3) - 42*a(n-4) for n > 5
 *            Initial values: a(1)=14, a(2)=86, a(3)=580, a(4)=4035,
 *            a(5)=27895
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the offset (first term is a(1)) */
    if (n == 0) return 0;              /* unused, but safe */

    /* Initial values for n = 1..5 */
    static const seq_t init[5] = {14, 86, 580, 4035, 27895};

    if (n <= 5)
        return init[n - 1];

    /* Keep the last four terms a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t prev[4] = {init[1], init[2], init[3], init[4]};

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(n) = 7*a(n-1) - 5*a(n-2) + 36*a(n-3) - 42*a(n-4) */
        seq_t sumPos = 7 * prev[3] + 36 * prev[1];   /* 7*a(n-1) + 36*a(n-3) */
        seq_t sumNeg = 5 * prev[2] + 42 * prev[0];   /* 5*a(n-2) + 42*a(n-4) */
        seq_t newVal = sumPos - sumNeg;              /* subtraction is safe: sumPos >= sumNeg */

        /* Shift the window for the next iteration */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = newVal;
    }

    return prev[3];
}
