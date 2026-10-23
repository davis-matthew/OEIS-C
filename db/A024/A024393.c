/*
 * A024393  4th elementary symmetric function of the first n+3 positive
 *          integers congruent to 2 mod 3.
 * https://oeis.org/A024393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(405*n^4+4590*n^3+18495*n^2+30534*n+16376)/1920
 * Range:     u8 none, u16 n <= 2, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   to_seq128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/to_seq128.h"

seq_t a(idx_t n)
{
    __uint128_t nn  = n;
    __uint128_t n2  = nn * nn;
    __uint128_t n3  = n2 * nn;
    __uint128_t n4  = n3 * nn;

    /* Polynomial part */
    __uint128_t poly = (405u * n4)
                     + (4590u * n3)
                     + (18495u * n2)
                     + (30534u * nn)
                     + 16376u;

    /* Product of the four consecutive numbers */
    __uint128_t t = nn * (nn + 1) * (nn + 2) * (nn + 3);

    /* Numerator and division by 1920 */
    __uint128_t num = t * poly;
    __uint128_t res = num / 1920u;

    return to_seq128(res);
}
