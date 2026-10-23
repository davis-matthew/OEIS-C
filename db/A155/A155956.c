/*
 * A155956  a(n) = Sum_{k=0..n} (k*n)^k.
 * https://oeis.org/A155956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k*n)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = (seq_t)k * (seq_t)n;
        seq_t term;
        if (k == 0) {
            /* 0^0 is defined as 1 for this sequence */
            term = 1;
        } else {
            term = big_pow(base, (seq_t)k);
        }
        total += term;
    }
    return total;
}
