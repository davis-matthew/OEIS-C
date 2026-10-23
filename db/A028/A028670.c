/*
 * A028670  Pseudo Galois numbers for d=8; order of group AGL(n,2^3).
 * https://oeis.org/A028670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n * Product_{k=0..n-1} (8^n - 8^k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t pow8_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow8_n *= (seq_t)8;

    seq_t result = pow8_n;
    seq_t pow_k   = (seq_t)1;
    for (idx_t k = 0; k < n; ++k) {
        result *= (pow8_n - pow_k);
        pow_k   *= (seq_t)8;
    }
    return result;
}
