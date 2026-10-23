/*
 * A038272  Triangle whose (i,j)-th entry is binomial(i,j)*7^(i-j)*6^j.
 * https://oeis.org/A038272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 7^(i-j) * 6^j where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* compute i = floor((sqrt(8*n+1)-1)/2) */
    idx_t sqrt_term = _floor_isqrt_u64(8*n + 1);
    idx_t i = (sqrt_term - 1) / 2;

    /* compute j = n - i*(i+1)/2 */
    unsigned __int128 mult = (unsigned __int128)i * (i + 1);
    idx_t cum_prev = (idx_t)(mult / 2);
    idx_t j = n - cum_prev;

    /* term = C(i,j) * 7^(i-j) * 6^j */
    seq_t bin = binomial(i, j);
    idx_t exp7 = i - j;
    seq_t p7  = pow_int((seq_t)7, exp7);
    seq_t p6  = pow_int((seq_t)6, j);
    return bin * p7 * p6;
}
