/*
 * A054331  One eighth of eighth unsigned column of Lanczos triangle A053125.
 * https://oeis.org/A054331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2^(2*n-3) * binomial(2*n+8, 7)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 8, u64 n <= 20, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* signed exponent: 2*n-3, may be negative for n = 0,1 */
    int64_t exp = (int64_t)2*n - 3;
    seq_t b = binomial(2*n + 8, 7);

    if (exp >= 0)
        return b << (idx_t)exp;     /* multiply by 2^exp */
    else
        return b >> (idx_t)(-exp);  /* divide by 2^(-exp)  */
}
