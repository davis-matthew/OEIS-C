/*
 * A225009  Number of n X 7 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>8 Initial values: a(1)=29,
 *            a(2)=239, a(3)=1163, a(4)=4166, a(5)=12174, a(6)=30738,
 *            a(7)=69498, a(8)=144111
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 42, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case n < 1 if called erroneously */
    if (n < 1)
        return (seq_t)0;

    /* Initial values a(1) .. a(8) */
    const seq_t init[8] = {
        (seq_t)29,
        (seq_t)239,
        (seq_t)1163,
        (seq_t)4166,
        (seq_t)12174,
        (seq_t)30738,
        (seq_t)69498,
        (seq_t)144111
    };

    /* For n <= 8 we can return the known value directly */
    if (n <= 8)
        return init[(size_t)(n - 1)];

    /* Use a sliding window of the last 8 terms */
    seq_t buf[8];
    for (size_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Iterate from 9 to n to compute the sequence */
    for (size_t i = 9; i <= (size_t)n; ++i) {
        seq_t cur =
            (seq_t)8   * buf[7]  // 8*a(i-1)
          - (seq_t)28  * buf[6]  // -28*a(i-2)
          + (seq_t)56  * buf[5]  // +56*a(i-3)
          - (seq_t)70  * buf[4]  // -70*a(i-4)
          + (seq_t)56  * buf[3]  // +56*a(i-5)
          - (seq_t)28  * buf[2]  // -28*a(i-6)
          + (seq_t)8   * buf[1]  // +8*a(i-7)
          - buf[0];             // -a(i-8)

        /* Shift the window: move buf[1..7] left, store cur in buf[7] */
        for (size_t j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = cur;
    }

    /* After the loop buf[7] holds a(n) */
    return buf[7];
}
