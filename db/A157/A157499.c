/*
 * A157499  Denominator of Euler(n, 1/31).
 * https://oeis.org/A157499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 31^n * 2^{v2(n+1)} (where v2(k) is the exponent of 2
 *            dividing k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute 31^n */
    seq_t pow31 = big_pow((seq_t)31, (seq_t)n);

    /* count trailing zeros of n+1  →  v2(n+1) */
    idx_t t = 0;
    idx_t m = n + 1;
    while ((m & ((idx_t)1)) == 0) {
        ++t;
        m >>= 1;
    }

    /* compute 2^t */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)t);

    /* result = 31^n * 2^t */
    return seq_mul(pow31, pow2);
}
