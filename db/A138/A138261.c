/*
 * A138261  Smallest positive integer m such that n divides [8^m/m]
 *          (=A129798(m)).
 * https://oeis.org/A138261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the smallest positive integer m such that n divides floor(8^m/m). */
#include <stdint.h>
#include "../utils/mul_mod64.h"

static uint64_t pow_mod_8(uint64_t exp, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t base = 8 % mod;
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    for (uint64_t m = 1; ; ++m) {
        uint64_t mod = m * N;
        uint64_t r = pow_mod_8(m, mod);
        uint64_t floor_val = r / m;
        if (floor_val % N == 0)
            return (seq_t)m;
    }
}
