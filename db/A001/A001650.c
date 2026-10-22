/*
 * A001650  k appears k times (k odd).
 * https://oeis.org/A001650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 2*floor(sqrt(n-1))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000037_isqrt.h"


seq_t a(idx_t n) {
    uint64_t sqrt_val = a000037_isqrt(n - 1);
    return (seq_t)(1 + 2 * sqrt_val);
}
