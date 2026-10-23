/*
 * A040424  Continued fraction for sqrt(446).
 * https://oeis.org/A040424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(446):
 * a(0) = 21
 * for n >= 1, a(n) = period[(n-1) % 16] where period = {8,2,2,1,3,1,1,20,1,1,3,1,2,2,8,42}
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)21;
    static const seq_t period[16] = {
        (seq_t)8, (seq_t)2, (seq_t)2, (seq_t)1,
        (seq_t)3, (seq_t)1, (seq_t)1, (seq_t)20,
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)1,
        (seq_t)2, (seq_t)2, (seq_t)8, (seq_t)42
    };
    return period[(n-1) % 16];
}
