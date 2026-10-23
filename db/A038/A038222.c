/*
 * A038222  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*4^j.
 * https://oeis.org/A038222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*3^(i-j)*4^j where i =
 *            floor((sqrt(1+8n)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find the row index i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    /* use 128‑bit arithmetic for safe multiplication */
    while (( (__int128)(i + 1) * (i + 2) ) <= ( (__int128)2 * n )) {
        i++;
    }

    /* Compute j = n - i*(i+1)/2 */
    idx_t tri = (idx_t)(( (__int128)i * (i + 1) ) / 2);
    idx_t j = n - tri;

    /* Compute term: binomial(i,j) * 3^(i-j) * 4^j */
    seq_t bin = binomial(i, j);
    seq_t p3  = pow3(i - j);
    seq_t p4  = pow4(j);

    return (bin * p3) * p4;
}
