/*
 * A208389  Number of n X 5 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 222*a(n-1) - 612*a(n-2) + 432*a(n-3), n>4 Offset : 1
 *            (a(1) = 41, a(2) = 8838, a(3) = 1937736, a(4) = 424785708)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Return known initial values directly */
    switch (n) {
    case 1: return (seq_t)41;
    case 2: return (seq_t)8838;
    case 3: return (seq_t)1937736;
    case 4: return (seq_t)424785708;
    default: break;
    }

    /* Setup the recurrence window:
       x = a(n-3), y = a(n-2), z = a(n-1)  (for the first step x=a2, y=a3, z=a4) */
    seq_t x = (seq_t)8838;          /* a(2) */
    seq_t y = (seq_t)1937736;       /* a(3) */
    seq_t z = (seq_t)424785708;     /* a(4) */

    /* Iterate from n = 5 up to the requested index */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)222 * z - (seq_t)612 * y + (seq_t)432 * x;
        /* Shift the values for the next iteration */
        x = y;
        y = z;
        z = next;
    }

    return z;
}
