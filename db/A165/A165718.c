/*
 * A165718  Integers of the form k*(k+7)/6.
 * https://oeis.org/A165718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + 14*n) / 8 if n is even a(n) = (3*n^2 + 16*n + 5)
 *            / 8 if n is odd This implements sequence A165718 (Integers of
 *            the form k*(k+7)/6)
 * Range:     u8 n <= 15, u16 n <= 257, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence type first to avoid undefined
       behavior on large indices. */
    seq_t sn = (seq_t)n;
    seq_t sn2 = sn * sn;          /* n^2 */

    if ((n & 1) == 0) {           /* even n */
        return ((seq_t)3 * sn2 + (seq_t)14 * sn) / (seq_t)8;
    } else {                      /* odd n */
        return ((seq_t)3 * sn2 + (seq_t)16 * sn + (seq_t)5) / (seq_t)8;
    }
}
