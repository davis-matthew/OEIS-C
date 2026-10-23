/*
 * A123097  Triangle read by rows: T(n,k) = binomial(n-2, k-1) +
 *          n*binomial(n-1, k-1), 1 <= k <= n, starting with T(1, 1) = 1.
 * https://oeis.org/A123097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle T(n,k) where T(n,k) =
 *            binomial(n-2, k-1) + n * binomial(n-1, k-1), read row‑by‑row, n
 *            ≥ 1
 * Range:     u8 n <= 31, u16 n <= 126, u32 n <= 479, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    if (n == 1)
        return (seq_t)1;          /* T(1,1) = 1 */

    /* Find the row r such that n lies in that row.
       The cumulative count up to row r is r*(r+1)/2. */
    idx_t r = 1;
    idx_t cum = 1;                /* r*(r+1)/2 for r = 1 */
    while (n > cum) {
        r++;
        cum += r;                 /* cum = r*(r+1)/2 for current r */
    }

    /* Elements before this row: (r-1)*r/2 = cum - r */
    idx_t prev_cum = cum - r;
    idx_t k = n - prev_cum;      /* 1 ≤ k ≤ r */

    /* Compute T(r,k).  For r=1 the term is already handled. */
    seq_t res = 0;

    if (r >= 2) {
        res += binomial(r - 2, k - 1);
    }

    seq_t part = binomial(r - 1, k - 1);
    res += (seq_t)r * part;

    return res;
}
