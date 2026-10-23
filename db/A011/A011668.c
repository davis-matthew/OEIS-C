/*
 * A011668  A binary m-sequence: expansion of reciprocal of x^6+x^5+x^2+x+1.
 * https://oeis.org/A011668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) XOR a(n-5) XOR a(n-2) XOR a(n-1) with initial
 *            values a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1 (the
 *            expansion of 1/(x^6+x^5+x^2+x+1) over GF(2))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n < 6) {
        static const seq_t init[6] = {0, 0, 0, 0, 0, 1};
        return init[n];
    }

    /* sliding window of the last six sequence values:
       index 0: a(n-6), 1: a(n-5), 2: a(n-4), 3: a(n-3),
       4: a(n-2), 5: a(n-1)   */
    seq_t prev[6] = {0, 0, 0, 0, 0, 1};
    seq_t cur = 0;

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) as XOR of the required taps */
        cur = prev[0] ^ prev[1] ^ prev[4] ^ prev[5];

        /* shift the window left by one position to prepare for next step */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }
    return cur;
}
