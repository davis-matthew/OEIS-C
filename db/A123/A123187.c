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
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

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
    seq_t bin = binomial(r, k);         /* C(r,k)          */
    seq_t pow = ipow_u64_idx(13, k);     /* 13^k            */
    return bin * pow;
}
