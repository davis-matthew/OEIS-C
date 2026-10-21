/*
 * A206706  Triangle read by rows, T(n,k) n>=0, 0<=k<=n; T(0,0) = -1 and for n
 *          > 0 T(n,k) = moebius(n,k+1) - moebius(n,k) where moebius(n,k) =
 *          mu(floor(n/k)) if k<>0 and k divides n, 0 otherwise; mu=A008683.
 * https://oeis.org/A206706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = T(row, col) with
   T(0,0) = -1,
   T(n,k) = moebius(n,k+1) - moebius(n,k)   for n>0,
   moebius(n,0) = 0,
   moebius(n,k) = μ(n/k) if k≠0 and k|n, else 0.
   The sequence is the flattened triangle (row by row). */

#include "../utils/mobius.h"

static seq_t moebius_block(idx_t n, idx_t k)
{
    if (k == 0 || n % k != 0)
        return (seq_t)0;
    return mobius(n / k);
}

seq_t a(idx_t n)
{
    /* Special first entry */
    if (n == 0)
        return (seq_t)-1;

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column within the row: 0 ≤ k ≤ r */
    idx_t prev_cum = r * (r + 1) / 2;
    idx_t k = n - prev_cum;

    /* Compute T(r,k) = moebius(r,k+1) – moebius(r,k)  */
    seq_t term1 = moebius_block(r, k + 1);
    seq_t term2 = moebius_block(r, k);
    return term1 - term2;
}
