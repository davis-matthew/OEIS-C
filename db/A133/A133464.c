/*
 * A133464  a(3n)=4^n, a(3n+1)=2*4^n, a(3n+2)=3*4^n.
 * https://oeis.org/A133464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n)=4^n, a(3n+1)=2*4^n, a(3n+2)=3*4^n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    idx_t mod = n % 3;
    idx_t k   = n / 3;
    seq_t pow4 = big_pow(4, (seq_t)k); /* 4^k */
    return pow4 * (seq_t)(mod + 1);
}
