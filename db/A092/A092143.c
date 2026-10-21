/*
 * A092143  Cumulative product of all divisors of 1..n.
 * https://oeis.org/A092143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} k^floor(n/k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t exp = n / k;
        seq_t term = pow_uint((uint64_t)k, exp);
        result = seq_mul(result, term);
    }
    return result;
}
