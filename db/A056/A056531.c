/*
 * A056531  Sequence remaining after a fourth round of Flavius Josephus sieve;
 *          remove every fifth term of A056530.
 * https://oeis.org/A056531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-12) – a(n-13) for n > 13 Initial values:
 *            a(1)=1, a(2)=3, a(3)=7, a(4)=13, a(5)=19, a(6)=25, a(7)=27,
 *            a(8)=31, a(9)=39, a(10)=43, a(11)=49, a(12)=51, a(13)=61
 * Range:     u8 n <= 52, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_vals[13] = {
    1, 3, 7, 13, 19, 25, 27, 31, 39, 43, 49, 51, 61
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0, return 0 */

    if (n <= 13)
        return init_vals[n - 1];

    seq_t w[13];
    idx_t i, j;
    for (i = 0; i < 13; ++i)
        w[i] = init_vals[i];

    seq_t val = 0;
    for (i = 14; i <= n; ++i)
    {
        /* a(i) = a(i-1) + a(i-12) – a(i-13) */
        val = w[12] + w[1] - w[0];

        /* shift window left: drop a(i-13), keep a(i-12)…a(i-1) */
        for (j = 0; j < 12; ++j)
            w[j] = w[j + 1];
        w[12] = val;                    /* new term becomes the last element */
    }
    return val;
}
