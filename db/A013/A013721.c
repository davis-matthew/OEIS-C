/*
 * A013721  a(n) = 16^(2*n + 1).
 * https://oeis.org/A013721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(2*n+1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t res = pow_uint(256, n);
    res = seq_mul((seq_t)16, res);
    return res;
}
