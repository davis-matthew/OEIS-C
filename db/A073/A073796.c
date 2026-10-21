/*
 * A073796  Replace 9^k with (-9)^k in base 9 expansion of n.
 * https://oeis.org/A073796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(9*k + m) = -9*a(k) + m (0 ≤ m < 9), a(0) = 0
 * Range:     i8 n <= 62, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* recursive step according to the recurrence relation */
    seq_t sub = a(n / 9);
    return -9 * sub + (seq_t)(n % 9);
}
