/*
 * A162014  Sequence related to the o.g.f.s. of the right hand columns of the
 *          EG1 triangle A162005.
 * https://oeis.org/A162014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^((n^2+n-2)/2)*4^((n-1)*n/2)*n!*Π_{k=0}^{n-1}(k!)^2
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 5, i128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* offset starts at 1 */

    /* sign factor */
    idx_t sign_exponent = ((n * n + n - 2) / 2) & 1;
    /* 4^((n-1)*n/2) */
    idx_t e4 = ((n - 1) * n) / 2;
    seq_t term4 = big_pow((seq_t)4, (seq_t)e4);

    /* product part = n! * Π_{i=1}^{n-1} i^{2(n-i)+1} */
    seq_t prod = (seq_t)n;   /* factor for i = n with exponent 1 */
    for (idx_t i = n - 1; i > 0; --i) {
        idx_t exp = 2 * (n - i) + 1;
        seq_t p = big_pow((seq_t)i, (seq_t)exp);
        prod *= p;
    }

    prod *= term4;
    if (sign_exponent)
        prod = -prod;
    return prod;
}
