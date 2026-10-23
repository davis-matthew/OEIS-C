/*
 * A104633  Triangle T(n,k) = k*(k-n-1)*(k-n-2)/2 read by rows, 1<=k<=n.
 * https://oeis.org/A104633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = row such that T(r,1)...T(r,r) are the
 *            nth terms of the triangle; k = n - r(r-1)/2. T(r,k) =
 *            k*(k-r-1)*(k-r-2)/2
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: O(√n) per call. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 1;               /* cumulative number of terms so far */
    while (cum < n) {
        r++;
        cum += r;
    }

    /* Position k in this row (1‑based). */
    idx_t k = n - ((r - 1) * r / 2);

    /* Compute k*(k-r-1)*(k-r-2)/2 using signed arithmetic to avoid
       underflow on negative factors. */
    long long kk = (long long)k;
    long long rr = (long long)r;
    long long d1 = kk - rr - 1;
    long long d2 = kk - rr - 2;
    long long prod = kk * d1 * d2 / 2;

    return (seq_t)prod;
}
