/*
 * A192466  Coefficient of x in the reduction by x^2->x+2 of the polynomial
 *          p(n,x)=1+x^n+x^(2n).
 * https://oeis.org/A192466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1 - (-1)^n + 2^n + 4^n) / 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n odd: a(n) = (2^n + 4^n) / 3 */
/* For n even: a(n) = (2^n + 4^n - 2) / 3 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t sum = pow2 + pow4;

    if ((n & 1) == 0) { /* n is even */
        sum -= (seq_t)2;
    }

    return sum / (seq_t)3;
}
