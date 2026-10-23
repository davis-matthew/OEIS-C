/*
 * A136577  Conjectured Hankel transform of A136576(n+1).
 * https://oeis.org/A136577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) =
 *            4^floor(n^2/4)*((1/2-sqrt(2)/2)*cos(3*Pi*n/4)+(1/2+sqrt(2)/2)*cos(Pi*n/4))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 7, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent floor(n^2/4) */
    uint64_t n64 = n;
    uint64_t exp_idx = (n64 * n64) / 4U;           /* safe for n up to ~10000 */
    seq_t exp = (seq_t)exp_idx;

    /* coefficient derived from n mod 8 */
    uint64_t mod8 = n & 7ULL;
    int coeff;
    switch (mod8) {
        case 0:
        case 1:
        case 7:
            coeff = 1;
            break;
        case 2:
        case 6:
            coeff = 0;
            break;
        default:        /* 3,4,5 */
            coeff = -1;
            break;
    }

    if (coeff == 0)
        return (seq_t)0;

    seq_t pow4 = big_pow((seq_t)4, exp);

    return coeff > 0 ? pow4 : -pow4;
}
