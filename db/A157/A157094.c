/*
 * A157094  Denominator of Euler(n, 1/27).
 * https://oeis.org/A157094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^v₂(n+1) * 3^(3n) where v₂(x) is the exponent of 2 in
 *            the prime factorisation of x
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* 3^(3n) */
    seq_t res = seq_pow((seq_t)3, (idx_t)(3 * n));

    /* Compute v₂(n+1) */
    idx_t m = n + 1;
    idx_t v2 = 0;
    while ((m & 1) == 0) {
        v2++;
        m >>= 1;
    }

    /* Multiply by 2^v₂(n+1) */
    seq_t factor = seq_pow((seq_t)2, v2);
    return seq_mul(res, factor);
}
