/*
 * A143418  Triangle read by rows. T(n,k) = binomial(n,k)*(binomial(n,k)-1)/2.
 * https://oeis.org/A143418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = C(r,k)*(C(r,k)-1)/2 where r is the unique integer ≥ 2
 *            with r(r-1)/2 ≥ n and k = n - (r-1)(r-2)/2 (1 ≤ k ≤ r-1)
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 178, u64 n <= 610,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes binomial coefficient C(n,k) using a multiplicative
   algorithm.  The routine assumes k ≤ n and works for all results
   that fit into the seq_t type.  */
static seq_t binomial_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the triangle row (r) and the position within that row (k). */
    idx_t row = 2;
    idx_t cum = 0;                       /* cumulative count before current row */
    while (cum + (row - 1) < n) {
        cum += row - 1;
        ++row;
    }

    idx_t k = n - cum;                    /* 1‑based index within the row */
    seq_t bin = binomial_calc(row, k);    /* C(row,k) */

    /* Compute T(row,k) = C(row,k)*(C(row,k)-1)/2 without overflow. */
    seq_t val;
    if (bin <= 1) {
        val = 0;
    } else if ((bin & (seq_t)1) == 0) {   /* bin even */
        val = (bin / 2) * (bin - 1);
    } else {                              /* bin odd */
        val = bin * ((bin - 1) / 2);
    }
    return val;
}
