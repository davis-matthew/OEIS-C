/*
 * A013499  a(n) = 2*n^n, n > 0, otherwise a(n) = 1.
 * https://oeis.org/A013499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^n for n>0, with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t val = big_pow((seq_t)n, (seq_t)n);
    val = mul_small(val, 2);
    return val;
}
