/*
 * A004480  Nim product 15 * n.
 * https://oeis.org/A004480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15 (*) n, the nim (Conway) product, computed by recursive
 *            field splitting: for X = 2^(2^k), (a1*X+a0)(*)(b1*X+b0) =
 *            (e+c)*X + (c + d(*)(X/2)), with c = a0(*)b0, d = a1(*)b1, e =
 *            (a0+a1)(*)(b0+b1), and + meaning XOR
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   nim_mul_level
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nim_mul_level.h"

seq_t a(idx_t n) {
    uint64_t r = nim_mul_level((uint64_t)15, (uint64_t)n, 6);
    seq_t hi = (seq_t)(idx_t)(r >> 32);
    seq_t lo = (seq_t)(idx_t)(r & 0xFFFFFFFFULL);
    return hi * (seq_t)65536 * (seq_t)65536 + lo;
}
