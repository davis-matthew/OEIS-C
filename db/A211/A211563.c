/*
 * A211563  Number of nonnegative integer arrays of length n+3 with new values
 *          0 upwards introduced in order, and containing the value n-1.
 * https://oeis.org/A211563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) for n > 7 Initial values (a(1) … a(7)): 15,
 *            51, 171, 512, 1345, 3145, 6676
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 75, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_seq[7] = {15, 51, 171, 512, 1345, 3145, 6676};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* value for n <= 0 is irrelevant */

    if (n <= 7)
        return init_seq[n - 1];

    /* ring buffer w[0] = a[i-1], w[1] = a[i-2], ..., w[6] = a[i-7] */
    seq_t w[7];
    for (size_t i = 0; i < 7; ++i)
        w[i] = init_seq[6 - i];

    for (idx_t i = 8; i <= n; ++i)
    {
        /* compute a[i] = 7*w[0] - 21*w[1] + 35*w[2] - 35*w[3]
                         + 21*w[4] - 7*w[5] + w[6] */
        seq_t next = 0;
        next += 7 * w[0];
        next += 35 * w[2];
        next += 21 * w[4];
        next += w[6];
        next -= 21 * w[1];
        next -= 35 * w[3];
        next -= 7 * w[5];

        /* shift the window: w[j] becomes w[j] for next iteration */
        for (size_t j = 6; j > 0; --j)
            w[j] = w[j - 1];
        w[0] = next;
    }

    return w[0];
}
