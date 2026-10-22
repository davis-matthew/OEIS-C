/*
 * A004475  Nim product 10 * n.
 * https://oeis.org/A004475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051775(10,n), the nim product 10 (x) n, computed by the
 *            Fermat-2-power recursion: for x = 2^h, (a1*x + a0)(x)(b1*x + b0)
 *            has high half (a0^a1)(x)(b0^b1) ^ a0(x)b0 and low half a0(x)b0 ^
 *            (a1(x)b1)(x)2^(h-1)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   nim_mul_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nim_mul_bits.h"

seq_t a(idx_t n) {
    uint64_t r = nim_mul_bits((uint64_t)10, (uint64_t)n, 64);
    return (seq_t)r;
}
