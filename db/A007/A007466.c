/*
 * A007466  Exponential-convolution of natural numbers with themselves.
 * https://oeis.org/A007466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*n + 2^(n-3)*(n-1)*(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   A001832_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/A001832_pow2.h"


seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t term1 = A001832_pow2(n - 1) * n_seq;
    
    if (n >= 3) {
        seq_t term2 = A001832_pow2(n - 3) * (n_seq - 1) * (n_seq - 2);
        return term1 + term2;
    } else {
        return term1;
    }
}
