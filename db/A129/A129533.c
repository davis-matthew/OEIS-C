/*
 * A129533  Array read by antidiagonals: T(n,k) =
 *          binomial(n+1,2)*binomial(n+k,n+1) for 0 <= k <= n.
 * https://oeis.org/A129533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) = antidiagonal_coords(n) and T(i,j) =
 *            binomial(i+1,2) * binomial(i+j,i+1)
 * Range:     u8 n <= 38, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    seq_t part1 = binomial(i + 1, 2);
    seq_t part2 = binomial(i + j, i + 1);
    return part1 * part2;
}
