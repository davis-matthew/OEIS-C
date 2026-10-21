/*
 * A209547  1/4 the number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock
 *          having exactly two distinct clockwise edge differences.
 * https://oeis.org/A209547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2) - 3*a(n-3) + 20*a(n-4) - 8*a(n-5) -
 *            2*a(n-6) Initial values: a(1)=20, a(2)=105, a(3)=562, a(4)=3051,
 *            a(5)=16582, a(6)=90186
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1; value for n = 0 is unused */

    /* constant initial values for n = 1 … 6 */
    static const seq_t init[6] = {20, 105, 562, 3051, 16582, 90186};

    if (n <= 6) return init[n - 1];

    /* sliding window of the last six terms: a_{k-5} … a_k */
    seq_t w[6];
    for (idx_t i = 0; i < 6; ++i)
        w[i] = init[i];    /* w[0] = a_1 , … , w[5] = a_6 */

    for (idx_t cur = 7; cur <= n; ++cur)
    {
        /* compute a_cur using the recurrence:
           a_cur = 8*a_{cur-1} - 14*a_{cur-2} - 3*a_{cur-3}
                    + 20*a_{cur-4} - 8*a_{cur-5} - 2*a_{cur-6}  */
        seq_t pos = 8 * w[5] + 20 * w[2];                      /* 8*a_{cur-1} + 20*a_{cur-4} */
        seq_t neg = 14 * w[4] + 3 * w[3] + 8 * w[1] + 2 * w[0];/* 14*a_{cur-2}+3*a_{cur-3}+8*a_{cur-5}+2*a_{cur-6} */
        seq_t next = pos - neg;                                   /* either unsigned or arbitrary‑precision */

        /* shift window left and insert new term */
        for (idx_t i = 0; i < 5; ++i)
            w[i] = w[i + 1];
        w[5] = next;
    }

    return w[5];
}
