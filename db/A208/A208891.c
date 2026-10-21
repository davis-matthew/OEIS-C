/*
 * A208891  Pascal's triangle matrix augmented with a right border of 1's.
 * https://oeis.org/A208891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n-th element in the row‑major flattening of the
 *            triangle T where T(r,r)=1 and T(r,k)=C(r−1,k) for k<r.
 *            Equivalent to a(n) = 1 if n ≡ (r+1)(r+2)/2 − 1 (border) C(r−1,k)
 *            otherwise where r = minimal integer with (r+1)(r+2)/2 > n and k
 *            = n − r(r+1)/2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r and column k such that
       a(n) = element at row r, column k. */
    idx_t cumulative = 0;   /* sum of sizes of previous rows */
    idx_t r = 0;            /* current row index */
    while (cumulative + (r + 1) <= n) {
        cumulative += r + 1;
        r++;
    }
    idx_t k = n - cumulative;   /* index within row r */

    /* If we hit the right border, the value is 1. */
    if (k == r) {
        return (seq_t)1;
    }

    /* Otherwise we need binomial coefficient C(r-1, k). */
    idx_t m = r - 1;         /* n in binomial */
    idx_t kk = k;
    if (kk > m - kk) {
        kk = m - kk;        /* use symmetry to reduce work */
    }

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= kk; ++i) {
        /* Multiply by (m - kk + i) and then divide by i.
           All intermediate calculations are performed in seq_t. */
        result = result * ((seq_t)(m - kk + i)) / ((seq_t)i);
    }
    return result;
}
