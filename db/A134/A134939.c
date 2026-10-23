/*
 * A134939  Numerator of the expected number of random moves in Tower of Hanoi
 *          problem with n disks starting on peg 1 and ending on peg 3.
 * https://oeis.org/A134939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (3^n-1)*(5^n-3^n)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t a = pow3 - (seq_t)1;
    seq_t b = pow5 - pow3;
    return (a * b) / (seq_t)2;
}
