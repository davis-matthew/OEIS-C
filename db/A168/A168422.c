/*
 * A168422  Number triangle with row sums given by quadruple factorial numbers
 *          A001813.
 * https://oeis.org/A168422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = value of the triangle entry at linear index n. The
 *            triangle is defined by T(n,k) = 1/k! * Σ_{j=k..n} (-1)^(j-k) *
 *            (2n-j)! / ((n-j)! (j-k)!) where 0 ≤ k ≤ n. a(n) is obtained by
 *            reading the triangle row‑major: row r starts at index r(r+1)/2
 *            and has entries T(r,0)…T(r,r)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 119, u128 n <= 324,
 *            big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/offset_row.h"

/* Return the starting index of row r (0‑based). */

/* Compute T(n,k) by the defining summation.  */
static seq_t T(idx_t n, idx_t k)
{
    if (n == 0 && k == 0)
        return (seq_t)1;

    /* factorial(k) */
    seq_t fact_k = 1;
    for (idx_t i = 2; i <= k; ++i)
        fact_k *= (seq_t)i;

    /* term_0 = product_{i=0}^{k-1} (n-i) = n!/(n-k)! */
    seq_t term = 1;
    for (idx_t i = 0; i < k; ++i)
        term *= (seq_t)(n - i);

    seq_t pos_sum = 0, neg_sum = 0;
    idx_t m = 0;

    /* sign for m=0: (-1)^(n-k-m) = (-1)^(n-k) */
    if ( ((n - k - m) & 1) == 0 )
        pos_sum += term;
    else
        neg_sum += term;

    /* iterate m = 1 .. n-k */
    for (m = 1; m <= n - k; ++m) {
        /* term_{m} = term_{m-1} * (n + m) * (n - k - (m-1)) / m */
        term *= (seq_t)(n + m);
        term *= (seq_t)(n - k - (m - 1));
        term /= (seq_t)m;

        /* sign for this term */
        if ( ((n - k - m) & 1) == 0 )
            pos_sum += term;
        else
            neg_sum += term;
    }

    seq_t summed = pos_sum - neg_sum;   /* summed ≥ 0 */
    return summed / fact_k;
}

/* Public sequence function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* first element of the triangle */

    /* find row r such that offset_row(r) ≤ n < offset_row(r+1) */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        ++r;

    idx_t k = n - offset_row(r);   /* column in the row */
    return T(r, k);
}
