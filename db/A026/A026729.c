/*
 * A026729  Square array of binomial coefficients T(n,k) = binomial(n,k), n >=
 *          0, k >= 0, read by downward antidiagonals.
 * https://oeis.org/A026729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) where (i, j) are the coordinates *
 *            obtained by reading Pascal’s triangle T(i,j)=C(i,j) in *
 *            antidiagonals. The first terms are * 1, 0, 1, 0, 1, 1, 0, 0, 2,
 *            1, …
 * Range:     u8 n <= 85, u16 n <= 395, u32 n <= 1112, u64 n <= 1224,
 *            u128 n <= 1224, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find the diagonal index s such that
       T_s = s*(s+1)/2 <= n < T_{s+1} */
    idx_t s = 0;
    while (((unsigned __int128)s * (s + 1)) / 2 <= n)
        ++s;
    --s;

    /* number of terms before this diagonal */
    idx_t prev = (idx_t)(((unsigned __int128)s * (s + 1)) / 2);

    /* position within this diagonal (0‑based) */
    idx_t pos = n - prev;          /* 0 <= pos <= s */

    /* coordinates in Pascal’s triangle:
       i increases from 0 to s, j decreases from s to 0. */
    idx_t i = pos;
    idx_t j = s - pos;

    return binom(i, j);            /* binom is provided in binom.h */
}
