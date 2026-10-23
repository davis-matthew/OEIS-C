/*
 * A167034  Triangle t(n,m)= (m+1)^n*binomial(n,m) if m <= n/2, otherwise
 *          t(n,m) = t(n,n-m).
 * https://oeis.org/A167034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = t(r,m) where r is the largest integer with r*(r+1)/2 ≤ n,
 *            m = n - r*(r+1)/2, and t(r,m) = (m+1)^r * binomial(r,m) (with
 *            symmetry t(r,m)=t(r,r-m) for m>r/2)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 by binary search. */
    idx_t r = 0;
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Index within the row. */
    seq_t tri_r = (seq_t)r * (r + 1) / 2;
    idx_t m = n - (idx_t)tri_r;
    if (m > r / 2) {
        m = r - m;  /* use symmetry */
    }

    /* Compute (m+1)^r and binomial(r,m). */
    seq_t power = pow_seq_large((seq_t)(m + 1), (seq_t)r);
    seq_t bin = binomial(r, m);

    return power * bin;
}
