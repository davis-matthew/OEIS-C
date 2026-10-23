/*
 * A194595  Triangle by rows T(n,k), showing the number of meanders with
 *          length (n+1)*3 and containing (k+1)*3 L's and (n-k)*3 R's, where
 *          L's and R's denote arcs of equal length and a central angle of 120
 *          degrees which are positively or negatively oriented.
 * https://oeis.org/A194595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 95, u64 n <= 309,
 *            u128 n <= 1100, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A194595 – flattened triangle.
   T(r,k) = C(r,k)^3 + C(r,k)*C(r,k+1)*C(r+1,k+1), 0 ≤ k ≤ r
   a(n) = the n‑th entry (row‑major) of T.                                                           */

#include <stdint.h>
#include "../utils/binom.h"          /* seq_t binom(idx_t n, idx_t k) */

static idx_t row_of(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
       Increments r until the next triangle number would exceed n. */
    idx_t r = 0;
    while (((__uint128_t)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    /* position k within the row, 0‑based */
    idx_t k = n - (idx_t)((__uint128_t)r * (r + 1) / 2);

    seq_t C_r_k   = binom(r, k);
    seq_t C_r_k1  = (k + 1 <= r) ? binom(r, k + 1) : 0;
    seq_t C_r1_k1 = binom(r + 1, k + 1);

    /* T(r,k) = C(r,k)^3 + C(r,k)*C(r,k+1)*C(r+1,k+1) */
    seq_t part1 = C_r_k * C_r_k * C_r_k;
    seq_t part2 = C_r_k * C_r_k1 * C_r1_k1;
    return part1 + part2;
}
