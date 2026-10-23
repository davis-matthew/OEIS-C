/*
 * A157707  The z^2 coefficients of the polynomials in the GF3 denominators of
 *          A156927 divided by 2.
 * https://oeis.org/A157707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = 7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+21*a(n-5)-7*a(n-6)+a(n-7)
   Initial values: a(1)=16, a(2)=205, a(3)=1165, a(4)=4415, a(5)=13055,
   a(6)=32606, a(7)=72030
   This implementation uses an iterative recurrence, O(n), with no dynamic allocation. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* First seven terms */
    static const seq_t init[7] = {16, 205, 1165, 4415, 13055, 32606, 72030};

    if (n <= 7) {
        return init[n - 1];
    }

    /* Buffer holding the seven most recent terms:
       buf[0] = a(n-7), buf[1] = a(n-6), ..., buf[6] = a(n-1)  */
    seq_t buf[7];
    for (int i = 0; i < 7; ++i) {
        buf[i] = init[i];
    }

    /* Compute the terms from a(8) up to a(n) inclusively */
    for (idx_t k = 8; k <= n; ++k) {
        /* Compute next term using only additions and a single subtraction
           to avoid unsigned underflow. */
        seq_t pos = 7 * buf[6] + 35 * buf[4] + 21 * buf[2] + buf[0];
        seq_t neg = 21 * buf[5] + 35 * buf[3] + 7 * buf[1];
        seq_t next = pos - neg;  /* guaranteed non‑negative for the sequence */

        /* Shift the buffer left by one position */
        for (int i = 0; i < 6; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[6] = next;
    }

    return buf[6];
}
