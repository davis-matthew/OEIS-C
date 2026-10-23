/*
 * A110314  Inverse of number triangle related to Fibonacci numbers.
 * https://oeis.org/A110314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 1 if n = T(k)-1 * a(n) = -(k-1) if n = T(k)-2 * a(n) =
 *            -(k-1)(k-2) if n = T(k)-3 * a(n) = 0 otherwise * where
 *            T(k)=k(k+1)/2 (triangular numbers) * This implements the
 *            recurrence of the inverse of the * number triangle defined in
 *            the OEIS entry
 * Range:     i8 n <= 87, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* x = n+1. We need the smallest k with T(k) >= x */
    idx_t x = n + 1;
    idx_t k = 0;
    /* find k by simple linear search (k grows ~ sqrt(2x)) */
    while ( (k * (k + 1) / 2) < x ) {
        k++;
    }
    idx_t T = k * (k + 1) / 2;
    idx_t d = T - x;               /* d = 0,1,2 ... */

    if (d == 0) {
        return (seq_t)1;
    } else if (d == 1) {
        /* -(k-1) */
        return -((seq_t)(k - 1));
    } else if (d == 2) {
        /* -(k-1)(k-2) */
        seq_t a1 = (seq_t)(k - 1);
        seq_t a2 = (seq_t)(k - 2);
        return -(a1 * a2);
    } else {
        return (seq_t)0;
    }
}
