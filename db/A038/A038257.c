/*
 * A038257  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*3^j.
 * https://oeis.org/A038257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 6^(i-j) * 3^j, where i =
 *            floor((sqrt(8*n+1)-1)/2), j = n - i*(i+1)/2
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    while (((unsigned __int128)(i + 1) * (i + 2) / 2) <= n) {
        ++i;
    }

    /* Compute the column j */
    idx_t pref = (idx_t)(((unsigned __int128)i * (i + 1)) / 2);
    idx_t j = n - pref;

    seq_t bin  = binomial(i, j);
    seq_t pow6 = ipow_u64_idx(6, i - j);
    seq_t pow3 = ipow_u64_idx(3, j);

    return bin * pow6 * pow3;
}
