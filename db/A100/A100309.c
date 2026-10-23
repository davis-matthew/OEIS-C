/*
 * A100309  Modulo 2 binomial transform of 6^n.
 * https://oeis.org/A100309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} mod(binomial(n, k), 2) * 6^k via the
 *            identity a(n) = ∏_{i:bit i set in n} (1 + 6^{1<<i})
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    idx_t tmp = n;
    idx_t i = 0;
    while (tmp) {
        if (tmp & 1) {
            seq_t p = pow_seq((seq_t)6, 1ULL << i);
            p += (seq_t)1;
            res *= p;
        }
        tmp >>= 1;
        ++i;
    }
    return res;
}
