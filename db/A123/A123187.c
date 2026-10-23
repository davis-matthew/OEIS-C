/*
 * A123187  Triangle of coefficients in expansion of (1 + 13*x)^n.
 * https://oeis.org/A123187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 13^k * C(r,k), where r = floor((sqrt(8*n+1)-1)/2) and k =
 *            n - r*(r+1)/2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        r++;

    /* offset of the first element of row r */
    idx_t offset = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    /* compute the coefficient */
    seq_t bin = binom(r, k);         /* C(r,k)          */
    seq_t pow = pow_uint(13, k);     /* 13^k            */
    return bin * pow;
}
