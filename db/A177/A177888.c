/*
 * A177888  P_n(k) with P_0(z) = z+1 and P_n(z) = z +
 *          P_(n-1)(z)*(P_(n-1)(z)-z) for n>1; square array P_n(k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A177888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = result of reading the square array P_n(k) by
 *            antidiagonals. Recurrence: P_0(z) = z+1 P_m(z) = z +
 *            P_{m-1}(z)*(P_{m-1}(z)-z) for m > 0
 * Range:     u8 n <= 17, u16 n <= 24, u32 n <= 32, u64 n <= 41, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute P_m(z) recursively. */
static seq_t compute_P(idx_t m, idx_t z)
{
    if (m == 0)          /* base case */
        return (seq_t)z + 1;
    seq_t prev = compute_P(m - 1, z);
    seq_t tmp  = prev - (seq_t)z;
    seq_t prod = prev * tmp;
    return (seq_t)z + prod;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find diagonal d such that (d+1)(d+2)/2 > n. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        ++d;

    /* Number of elements before diagonal d (triangular number). */
    idx_t t = d * (d + 1) / 2;
    idx_t k = n - t;               /* position within diagonal. */
    idx_t row = k;                 /* row index m */
    idx_t col = d - k;             /* column index z */

    /* Evaluate the polynomial P_row(col). */
    return compute_P(row, col);
}
