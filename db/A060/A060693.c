/*
 * A060693  Triangle (0 <= k <= n) read by rows: T(n, k) is the number of
 *          Schröder paths from (0,0) to (2n,0) having k peaks.
 * https://oeis.org/A060693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) where we read the triangle of Schröder paths
 *            T(r,k) by rows. For a given index n, compute the row r such that
 *            the r‑th row starts at index r*(r+1)/2 and ends before
 *            (r+1)*(r+2)/2. Then k = n – r*(r+1)/2. The entry is T(r,k) =
 *            binomial(2*r - k, k) * Catalan(r - k)
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 140, u64 n <= 470,
 *            u128 n <= 1606, big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    idx_t start = 0;
    while (start <= n) {
        ++r;
        start = r * (r + 1) / 2;
    }
    --r;                                    /* now r satisfies r*(r+1)/2 <= n */
    start = r * (r + 1) / 2;
    idx_t k = n - start;
    seq_t val = binom(2 * r - k, k);
    val *= catalan(r - k);
    return val;
}
