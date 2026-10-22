/*
 * A004947  a(n) = round(n*phi^12), where phi is the golden ratio, A001622.
 * https://oeis.org/A004947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^12) = floor((322*n + 1 +
 *            floor(144*sqrt(5)*n))/2), since phi^12 = 161 + 72*sqrt(5) and
 *            floor(144*sqrt(5)*n) = isqrt(103680*n^2)
 * Range:     u8 n <= 0, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   calc_seq, calc_u128, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/calc_seq.h"
#include "../utils/calc_u128.h"

seq_t a(idx_t n) {
    if (seq_is_big()) return calc_seq(n);
    return u128_to_seq(calc_u128(n));
}
