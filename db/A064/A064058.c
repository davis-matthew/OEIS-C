/*
 * A064058  Ninth column of quintinomial coefficients.
 * https://oeis.org/A064058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+2,2) + 12*binomial(n+2,3) + 31*binomial(n+2,4)
 *            + 35*binomial(n+2,5) + 21*binomial(n+2,6) + 7*binomial(n+2,7) +
 *            binomial(n+2,8)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t r = 0;
    r += binomial(n + 2, 2);
    r += binomial(n + 2, 3) * 12;
    r += binomial(n + 2, 4) * 31;
    r += binomial(n + 2, 5) * 35;
    r += binomial(n + 2, 6) * 21;
    r += binomial(n + 2, 7) * 7;
    r += binomial(n + 2, 8);
    return r;
}
