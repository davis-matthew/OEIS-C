/*
 * A123276  Discriminant of the Chebyshev polynomial of first kind of degree
 *          2^n.
 * https://oeis.org/A123276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4^n + (n+1)*2^n - 3*2^n + 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = 2^(4^n + (n‑2)*2^n + 1) */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2*n) */
    seq_t term1 = pow2((idx_t)(2 * n));

    /* 2^n */
    seq_t pow2n = pow2(n);

    seq_t exponent;
    if (n >= 2) {
        /* (n-2)*2^n, n >= 2 so the multiplier is non‑negative */
        seq_t mult = mul_small(pow2n, (unsigned int)(n - 2));
        exponent = term1 + mult + (seq_t)1;
    } else {
        /* For n = 0 or 1 the multiplier (n-2) is negative.
           Exponent = 4^n + 1 - (2-n)*2^n                */
        seq_t mult = mul_small(pow2n, (unsigned int)(2 - n));
        exponent = term1 + (seq_t)1 - mult;
    }

    /* Return 2^exponent */
    return big_pow((seq_t)2, exponent);
}
