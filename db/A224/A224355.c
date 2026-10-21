/*
 * A224355  Number of 4 X n 0..2 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A224355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for
 *            n>12 * Initial values (a(1) to a(12)): * a(1)=81 a(2)=1296
 *            a(3)=5880 a(4)=19608 a(5)=57387 * a(6)=151010 a(7)=363392
 *            a(8)=810436 a(9)=1693423 a(10)=3344982 * a(11)=6292120
 *            a(12)=113...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 27, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Initial 12 terms */
    static const seq_t init[12] = {
        81ULL,      1296ULL,      5880ULL,     19608ULL,     57387ULL,
        151010ULL,  363392ULL,    810436ULL,   1693423ULL,   3344982ULL,
        6292120ULL, 11340202ULL
    };

    if (n <= 12)
        return init[n - 1];

    /* Buffer holds a(n-1) ... a(n-9) for current n */
    seq_t buf[9];
    buf[0] = init[11];  /* a(12) */
    buf[1] = init[10];  /* a(11) */
    buf[2] = init[9];   /* a(10) */
    buf[3] = init[8];   /* a(9)  */
    buf[4] = init[7];   /* a(8)  */
    buf[5] = init[6];   /* a(7)  */
    buf[6] = init[5];   /* a(6)  */
    buf[7] = init[4];   /* a(5)  */
    buf[8] = init[3];   /* a(4)  */

    seq_t next;
    for (idx_t cur = 13; cur <= n; ++cur) {
        /* Compute a(cur) using the recurrence */
        next = 9 * buf[0]
             - 36 * buf[1]
             + 84 * buf[2]
             - 126 * buf[3]
             + 126 * buf[4]
             - 84 * buf[5]
             + 36 * buf[6]
             - 9 * buf[7]
             + buf[8];

        if (cur == n)
            return next;

        /* Shift buffer: buf[1] becomes buf[0], ..., buf[8] becomes buf[7] */
        for (int k = 8; k > 0; --k)
            buf[k] = buf[k - 1];
        buf[0] = next;
    }
    /* Unreachable, but satisfy compiler */
    return 0;
}
