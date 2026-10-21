/*
 * A072405  Triangle T(n, k) = C(n,k) - C(n-2,k-1) for n >= 3 and T(n, k) = 1
 *          otherwise, read by rows.
 * https://oeis.org/A072405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = C(r,k) – C(r‑2,k‑1) for r ≥ 3; all
 *            other entries of the triangle are 1. The pair (r,k) is obtained
 *            from the flat index n by reading the triangle row‑by‑row with
 *            offset 0
 * Range:     u8 n <= 39, u16 n <= 197, u32 n <= 680, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* locate the row r: r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column index k within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* all entries for r < 3 are 1 (rows 0,1,2) */
    if (r < 3)
        return (seq_t)1;

    /* edge entries are 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* main formula: C(r,k) – C(r‑2,k‑1) */
    seq_t big   = binom(r, k);
    seq_t small = binom(r - 2, k - 1);  /* k ≥ 1 and r ≥ 2 here */
    return big - small;
}
