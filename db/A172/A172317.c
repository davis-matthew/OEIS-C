/*
 * A172317  8th column of A172119.
 * https://oeis.org/A172317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-8), a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16, a(5)=32, a(6)=64, a(7)=128
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base[8] = {1, 2, 4, 8, 16, 32, 64, 128};

seq_t a(idx_t n) {
    /* Return the pre‑computed base value for n < 8. */
    if (n < 8) {
        return base[n];
    }

    /* Use a sliding window of the last eight values to advance the
       recurrence to the requested index. */
    seq_t win[8];
    win[0] = base[0];
    win[1] = base[1];
    win[2] = base[2];
    win[3] = base[3];
    win[4] = base[4];
    win[5] = base[5];
    win[6] = base[6];
    win[7] = base[7];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = (seq_t)(2 * win[7] - win[0]);

        /* Shift the window */
        for (int j = 0; j < 7; ++j) {
            win[j] = win[j + 1];
        }
        win[7] = next;
    }

    return win[7];
}
