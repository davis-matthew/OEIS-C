/*
 * A060098  Triangle of partial sums of column sequences of triangle A060086,
 *          read by rows.
 * https://oeis.org/A060098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the row‑major flattening of the triangle T(n,k) given by
 *            T(n,k) = Σ_{i=0..⌊n/2⌋} C(n-2i, k) · C(k+i-1, i) where C(a,b) is
 *            the binomial coefficient (zero if b>a or b<0). For a given n,
 *            find the unique row r with r(r+1)/2 ≤ n < (r+1)(r+2)/2, and let
 *            k = n - r(r+1)/2. Then a(n)=T(r,k)
 * Range:     u8 n <= 39, u16 n <= 142, u32 n <= 506, u64 n <= 1734,
 *            u128 n <= 5150, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

/* Compute T(r,k) using the explicit sum formula. */
static seq_t T_value(idx_t r, idx_t k)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= r / 2U; ++i)
    {
        idx_t n2i = r - 2 * i;
        if (k > n2i)
            break;
        seq_t term1 = binom(n2i, k);           /* C(r-2i, k)      */
        seq_t term2 = binom((k + i) - 1, i);  /* C(k+i-1, i)  */
        sum += term1 * term2;
    }
    return sum;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return T_value(r, k);
}
