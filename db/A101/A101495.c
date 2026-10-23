/*
 * A101495  Column 1 of triangle A101494.
 * https://oeis.org/A101495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1)*(n-k)^k
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base  = (seq_t)(n - k);
        seq_t power = pow_seq(base, k);
        seq_t term  = (seq_t)(k + 1) * power;
        result += term;
    }
    return result;
}
