/*
 * A088459  Triangle read by rows: T(n,k) represents the number of lozenge
 *          tilings of an (n,1,n)-hexagon which include the non-vertical tile
 *          above the main diagonal starting in position k+1.
 * https://oeis.org/A088459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = T(r,k) where r = smallest integer with r*(r+1) >= n, k =
 *            n - r*(r-1) - 1, and T(r,k) = C(r, ceil(k/2)) * C(r-1,
 *            floor(k/2)) The sequence is the rows of the triangle of A088459
 *            read consecutively
 * Range:     u8 n <= 46, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; behavior below 1 is irrelevant */
        return 0;

    /* Find the row r such that r*(r+1) >= n. */
    idx_t r = 1;
    while (r * (r + 1) < n)
        ++r;

    /* Position within the row (0‑based). */
    idx_t prev = (r - 1) * r;          /* sum of lengths of rows 1..r-1 */
    idx_t k = n - prev - 1;            /* 0 ≤ k < 2*r */

    /* Ceil(k/2) and floor(k/2) */
    idx_t half = k >> 1;
    idx_t ceil_k_over_2 = (k & 1) ? half + 1 : half;
    idx_t floor_k_over_2 = half;

    /* Binomial coefficients for the definition. */
    seq_t left  = binom(r,          ceil_k_over_2);
    seq_t right = binom(r - 1,      floor_k_over_2);

    return left * right;
}
