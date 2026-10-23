/*
 * A180975  Array of the "egg-drop" numbers, read by downwards antidiagonals.
 * https://oeis.org/A180975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = Σ_{j=1..k} binomial(n,j) The
 *            sequence is read by downwards antidiagonals of the array T(n,k)
 * Range:     u8 n <= 86, u16 n <= 316, u32 n <= 1161, u64 n <= 4530,
 *            u128 n <= 5050, big any n
 * Helpers:   binom_mul_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_mul_div.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* Find the diagonal index s such that the total number of elements up to that
       diagonal is at least n.  The number of elements up to diagonal s is
       triangular(s-1) = (s-1)*s/2.  Use 128‑bit arithmetic to avoid overflow. */
    idx_t s = 2;
    while ( ((unsigned __int128)s * (s-1)) / 2 < n ) ++s;

    /* Compute the vertex index preceding diagonal s and the position within the
       current diagonal (1‑based). */
    idx_t prev = (idx_t)(((unsigned __int128)(s-2) * (s-1)) / 2);
    idx_t pos  = n - prev;          /* position within diagonal s; 1 ≤ pos ≤ s-1 */

    idx_t row = pos;                /* n in T(n,k) */
    idx_t col = s - pos;            /* k in T(n,k) */

    /* We only need to sum up to j = min(row, col) because binomial(row,j)=0 for j>row. */
    idx_t maxj = (col < row) ? col : row;

    seq_t result = 0;
    seq_t binom = 1; /* binomial(row,0) */
    for (idx_t j = 1; j <= maxj; ++j) {
        binom = binom_mul_div(binom, row - j + 1, j); /* compute binomial(row,j) */
        result += binom;
    }
    return result;
}
