/*
 * A100310  Modulo 2 binomial transform of 7^n.
 * https://oeis.org/A100310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n,k) mod 2) * 7^k Simplified: a(n)
 *            = ∏_{i: bit i of n set} (1 + 7^(2^i))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t bit = 0;
    idx_t m = n;
    while (m) {
        if (m & 1ULL) {
            /* Compute 7^(2^bit) */
            seq_t p = pow_seq((seq_t)7, ((idx_t)1 << bit));
            result *= (seq_t)1 + p;
        }
        m >>= 1;
        bit++;
    }
    return result;
}
