/*
 * A155798  Triangle read by rows: t(n,k)=Binomial[n, k] + Binomial[k*(n - k),
 *          n].
 * https://oeis.org/A155798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For the triangle t(n,k) = Binomial[n,k] + Binomial[k*(n-k),n]
 *            (n,k ≥ 0), the sequence a(n) is the row‑major reading of the
 *            triangle with a(0)=2, a(1)=1, a(2)=1, … as listed in the OEIS
 *            entry
 * Range:     u8 n <= 29, u16 n <= 39, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* determine the row r and column k such that n lies in the nth element
       of the row‑major reading of the triangle:
       cumulative elements up to row r-1 is r*(r+1)/2.
       Find the smallest r with (r+1)*(r+2)/2 > n. */
    idx_t low = 0, high = 1;
    while ((high + 1) * (high + 2) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((mid + 1) * (mid + 2) / 2 > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* row index  (0‑based) */
    idx_t cum_before = r * (r + 1) / 2;  /* elements before this row */
    idx_t k    = n - cum_before;         /* column index in this row */

    /* compute the triangle entry:  t(r,k) = Binomial[r,k] + Binomial[k*(r-k),r] */
    seq_t part1 = binomial((idx_t)r, k);
    idx_t m = k * (r - k);               /* k*(r-k) fits in idx_t for all n up to ~2^32 */
    seq_t part2 = binomial(m, r);
    return part1 + part2;
}
