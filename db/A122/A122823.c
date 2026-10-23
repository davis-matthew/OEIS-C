/*
 * A122823  Triangle read by rows: T(n,k) = count of increasing runs in two
 *          concatenated k-permutations of [n].
 * https://oeis.org/A122823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((k+1) - (n-1)/(2n)) * Permute(n,k)^2 for the triangle
 *            read by rows: T(1,1), T(2,1), T(2,2), T(3,1),
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    /* find row r such that r*(r+1)/2 >= n  (triangular numbers) */
    idx_t r = 1;
    idx_t tri = 1;                        /* r*(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }
    idx_t prev_tri = r * (r - 1) / 2;     /* sum of first (r-1) rows */
    idx_t k = n - prev_tri;               /* 1‑based column index in row r */

    /* Permutation P(r,k) = r!/(r-k)! */
    seq_t P = 1;
    for (idx_t i = 0; i < k; ++i) {
        P *= (seq_t)(r - i);
    }

    /* numerator and denominator of the rational factor */
    seq_t two_r = (seq_t)(2 * r);
    seq_t numerator = (seq_t)(k + 1) * two_r - (seq_t)(r - 1);
    seq_t denom = two_r;

    /* T(r,k) = P^2 * numerator / denom */
    seq_t result = (P * P * numerator) / denom;
    return result;
}
