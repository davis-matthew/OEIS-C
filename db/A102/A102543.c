/*
 * A102543  Antidiagonal sums of the antidiagonals of Losanitsch's triangle.
 * https://oeis.org/A102543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A102543: Antidiagonal sums of the antidiagonals of Losanitsch's triangle.
   Recurrence (for n ≥ 9):
      a(n) = a(n-1) + a(n-2) - a(n-5) + a(n-6) - a(n-7) - a(n-9)
   Initial values:
      a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=3,
      a(6)=4, a(7)=6, a(8)=8
   The implementation computes one value on demand, using only a small
   fixed-size ring buffer. No external state or advanced data structures
   are required. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* Initial values for n = 0..8 */
    static const seq_t init[9] = {1, 1, 1, 2, 2, 3, 4, 6, 8};

    /* Return immediately if within the initial range */
    if ((idx_t)8 >= n) {
        return init[(size_t)n];
    }

    /* ring buffer of the last 9 computed terms */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    /* Compute terms from 9 up to n */
    for (idx_t i = 9; i <= n; ++i) {
        /* Unroll modulo 9 indices */
        size_t im1 = (size_t)(i - 1) % 9;
        size_t im2 = (size_t)(i - 2) % 9;
        size_t im5 = (size_t)(i - 5) % 9;
        size_t im6 = (size_t)(i - 6) % 9;
        size_t im7 = (size_t)(i - 7) % 9;
        size_t im9 = (size_t)(i - 9) % 9;

        seq_t val = buf[im1] + buf[im2];
        if (i >= 5) val -= buf[im5];
        if (i >= 6) val += buf[im6];
        if (i >= 7) val -= buf[im7];
        if (i >= 9) val -= buf[im9];

        buf[(size_t)i % 9] = val;
    }

    return buf[(size_t)n % 9];
}
