/*
 * A206004  Number of (n+1) X 3 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one, and every 2 X 2
 *          determinant nonzero.
 * https://oeis.org/A206004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A206004
   a(n) = 2*a(n-1) + 13*a(n-2) - 17*a(n-3) - 34*a(n-4) - 9*a(n-5)
          + 23*a(n-6) + a(n-7) - 4*a(n-8) + a(n-9)
   (valid for n >= 10, with a(1)=78, a(2)=300, a(3)=980,
    a(4)=3706, a(5)=12304, a(6)=45810, a(7)=154346,
    a(8)=567112, a(9)=1932816) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First 9 terms, according to the OEIS entry */
    static const seq_t init[9] = {
        78, 300, 980, 3706, 12304, 45810, 154346, 567112, 1932816
    };

    if (n <= 9) {
        /* n is 1-indexed, array is 0-indexed */
        return init[n - 1];
    }

    /* Sliding window for the last 9 computed terms:
       a[0] = a(n-9), a[1] = a(n-8), ..., a[8] = a(n-1)   */
    seq_t a[9];
    for (int i = 0; i < 9; ++i) {
        a[i] = init[i];
    }

    seq_t cur = 0;
    for (idx_t i = 10; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t pos = 2 * a[8] + 13 * a[7] + 23 * a[3] + a[2] + a[0];
        seq_t neg = 17 * a[6] + 34 * a[5] + 9 * a[4] + 4 * a[1];
        cur = pos - neg;

        /* Shift the window for the next iteration */
        for (int t = 0; t < 8; ++t) {
            a[t] = a[t + 1];
        }
        a[8] = cur;
    }

    return cur;
}
