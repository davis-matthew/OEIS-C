/*
 * A064838  a(n) = A064837(n)/2.
 * https://oeis.org/A064838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 5*a(n-3) + 5*a(n-4) - 9*a(n-5) +
 *            5*a(n-6) - a(n-7) Initial values: a(0)=0, a(1)=1, a(2)=9,
 *            a(3)=57, a(4)=210, a(5)=585, a(6)=1355
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 132, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..6 (constants may overflow seq_t; that is acceptable
       because terms beyond the capacity are not required to be exact). */
    static const seq_t base[7] = {0, 1, 9, 57, 210, 585, 1355};

    if (n < 7) return base[(size_t)n];

    /* Sliding window of the last seven values. */
    seq_t win[7] = {
        base[0], base[1], base[2], base[3], base[4], base[5], base[6]
    };

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next =
            win[6] * 5            /* 5*a(n-1) */
          - win[5] * 9            /* -9*a(n-2) */
          + win[4] * 5            /* +5*a(n-3) */
          + win[3] * 5            /* +5*a(n-4) */
          - win[2] * 9            /* -9*a(n-5) */
          + win[1] * 5            /* +5*a(n-6) */
          - win[0];               /* -a(n-7)  */

        /* Shift the window and insert the newly computed value. */
        for (int k = 0; k < 6; ++k) win[k] = win[k + 1];
        win[6] = next;
    }

    return win[6];
}
