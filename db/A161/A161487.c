/*
 * A161487  Positive numbers y such that y^2 is of the form x^2+(x+191)^2 with
 *          integer x.
 * https://oeis.org/A161487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161487
   a(1) = 149, a(2) = 191, a(3) = 269, a(4) = 625,
   a(5) = 955, a(6) = 1465
   For n > 6: a(n) = 6 * a(n-3) - a(n-6)
   This implementation evaluates the recurrence directly for any n ≥ 1,
   using a small circular buffer to keep the last six terms. No dynamic
   allocation is required. */

#include <stdint.h>

static const seq_t init_values[6] = {149, 191, 269, 625, 955, 1465};

seq_t a(idx_t n)
{
    /* Return the first six terms directly. */
    if (n <= 6)
        return init_values[n - 1];

    /* Circular buffer for the last six terms. */
    seq_t buf[6];
    for (int i = 0; i < 6; ++i)
        buf[i] = init_values[i];

    /* Compute terms from 7 up to n. */
    for (idx_t i = 7; i <= n; ++i) {
        /* Indices in the circular buffer:
           idx_new  = position of a(i)
           idx_a3   = position of a(i-3)
           idx_a6   = position of a(i-6)   */
        int idx_new = (int)((i - 1) % 6);
        int idx_a3  = (int)((i - 4) % 6);
        int idx_a6  = (int)((i - 7) % 6);

        buf[idx_new] = 6 * buf[idx_a3] - buf[idx_a6];
    }

    /* The desired term is now in buf[(n-1)%6]. */
    return buf[(int)((n - 1) % 6)];
}
