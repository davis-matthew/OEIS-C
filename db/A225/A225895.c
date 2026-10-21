/*
 * A225895  Number of n X 3 binary arrays whose sum with another n X 3 binary
 *          array containing no more than a single 1 has rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A225895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n>9.
 *            Initial values: a(1)=7, a(2)=33, a(3)=145, a(4)=545, a(5)=1770,
 *            a(6)=5052, a(7)=12910, a(8)=30055, a(9)=64701
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* cache initial terms in a circular buffer of length 9 */
    seq_t buf[9] = {7, 33, 145, 545, 1770, 5052, 12910, 30055, 64701};

    if (n <= 9) {
        return buf[(size_t)(n - 1)];
    }

    size_t N = (size_t)n;
    for (size_t i = 10; i <= N; ++i) {
        size_t idx  = (i - 1) % 9;    /* index for a(i)  */
        size_t a1i  = (i - 2) % 9;    /* a(i-1)          */
        size_t a2i  = (i - 3) % 9;    /* a(i-2)          */
        size_t a3i  = (i - 4) % 9;    /* a(i-3)          */
        size_t a4i  = (i - 5) % 9;    /* a(i-4)          */
        size_t a5i  = (i - 6) % 9;    /* a(i-5)          */
        size_t a6i  = (i - 7) % 9;    /* a(i-6)          */
        size_t a7i  = (i - 8) % 9;    /* a(i-7)          */
        size_t a8i  = (i - 9) % 9;    /* a(i-8)          */
        size_t a9i  = (i -10) % 9;    /* a(i-9)          */

        /* Compute positive and negative parts separately to avoid underflow */
        seq_t pos = 9   * buf[a1i]
                  + 84  * buf[a3i]
                  + 126 * buf[a5i]
                  + 36  * buf[a7i]
                  +     buf[a9i];

        seq_t neg = 36  * buf[a2i]
                  + 126 * buf[a4i]
                  + 84  * buf[a6i]
                  + 9   * buf[a8i];

        buf[idx] = pos - neg;
    }
    return buf[(N - 1) % 9];
}
