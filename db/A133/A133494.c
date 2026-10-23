/*
 * A133494  Diagonal of the array of iterated differences of A047848.
 * https://oeis.org/A133494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=3^(n-1) for n>0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return pow3(n - 1);
}
