/*
 * A167004  Least possible nonnegative coefficients of x^n in G(x)^(3^n),
 *          n>=0, such that G(x) is an integer series with G'(0)=G(0)=1; the
 *          G(x) that satisfies this condition equals the g.f. of A167003.
 * https://oeis.org/A167004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; a(n) = 0 if n is not a power of 3; otherwise
 *            a(3^k) = 3^(3^k - k). This follows from the definition of the
 *            sequence
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 26, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t temp = n;
    idx_t k = 0;

    /* Test whether n is a power of 3 and count the exponent k */
    while (temp > 1) {
        if (temp % 3 != 0)
            return (seq_t)0;
        temp /= 3;
        ++k;
    }

    /* n is now 3^k.  Compute the exponent 3^k - k (using n for 3^k). */
    seq_t exponent = (seq_t)n - (seq_t)k;

    /* Return 3^(3^k - k) using the provided big_pow routine. */
    return big_pow((seq_t)3, exponent);
}
