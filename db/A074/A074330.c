/*
 * A074330  a(n) = Sum_{k=1..n} 2^b(k) where b(k) denotes the number of 1's in
 *          the binary representation of k.
 * https://oeis.org/A074330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1}^n 2^{popcount(k)} where popcount(k) is the number
 *            of * 1's in the binary representation of k
 * Range:     u8 n <= 34, u16 n <= 1208, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned pc = __builtin_popcountll(k);
        sum += pow2((idx_t)pc);
    }
    return sum;
}
