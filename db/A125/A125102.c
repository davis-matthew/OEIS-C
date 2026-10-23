/*
 * A125102  Triangle read by rows: T(n,k)=(k+1)binomial(n,k) +
 *          [3-(-1)^k]binomial(n,k+1)/2 (0<=k<=n).
 * https://oeis.org/A125102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattening of the triangle T(n,k) = (k+1)*C(n,k) +
 *            ((k%2==0)?1:2)*C(n,k+1) where C(n,k) is the binomial
 *            coefficient. The pair (n,k) for a given index n is found by:
 *            find the smallest r such that (r+1)(r+2)/2 > n. Then k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 38, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* locate the row r that contains index n.
       Row r has r+1 elements, cumulative up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    idx_t prev = r * (r + 1) / 2;      /* elements before this row */
    idx_t k    = n - prev;             /* column within row (0‑based) */

    seq_t bin_k  = binomial(r, k);
    seq_t part1  = ((seq_t)(k + 1)) * bin_k;

    seq_t part2  = 0;
    if (k + 1 <= r) {
        seq_t bin_k1 = binomial(r, k + 1);
        seq_t factor = (k & 1) ? (seq_t)2 : (seq_t)1;
        part2 = factor * bin_k1;
    }

    return part1 + part2;
}
