/*
 * A232117  Number of (n+1) X (1+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or vertical neighbor.
 * https://oeis.org/A232117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5*a(n-1) +4*a(n-2) +12*a(n-3)
 *             -7*a(n-4) -14*a(n-5) -33*a(n-6) -22*a(n-7)
 *             +4*a(n-8) +4*a(n-9)
 * Initial values:
 *   a(1)=15, a(2)=109, a(3)=611, a(4)=3635,
 *   a(5)=21717, a(6)=129323, a(7)=770747,
 *   a(8)=4593729, a(9)=27378111
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base terms a(1) .. a(9) */
    static const seq_t base[9] = {
        (seq_t)15,
        (seq_t)109,
        (seq_t)611,
        (seq_t)3635,
        (seq_t)21717,
        (seq_t)129323,
        (seq_t)770747,
        (seq_t)4593729,
        (seq_t)27378111
    };

    if (n == 0) {
        /* Offset is 1; a(0) is undefined, return 0 for safety */
        return (seq_t)0;
    }

    if (n <= 9) {
        return base[n - 1];
    }

    /* Sliding window of the last 9 values: indices 0..8
     * buffer[0] = a(i-9), ..., buffer[8] = a(i-1) for current i */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) buf[i] = base[i];

    seq_t pos, neg, new_val;
    for (idx_t i = 10; i <= n; ++i) {
        /* Compute positive and negative parts separately
           to avoid subtraction underflow on unsigned types      */
        pos = 5 * buf[8] + 4 * buf[7] + 12 * buf[6]
            + 4 * buf[0] + 4 * buf[1];
        neg = 7 * buf[5] + 14 * buf[4] + 33 * buf[3]
            + 22 * buf[2];
        new_val = pos - neg;

        /* Shift buffer left by one: discard a(i-9) */
        for (idx_t j = 0; j < 8; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[8] = new_val;            /* a(i) now in buf[8]  */
    }

    return buf[8];
}
