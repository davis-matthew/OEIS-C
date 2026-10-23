/*
 * A038285  Triangle whose (i,j)-th entry is binomial(i,j)*8^(i-j)*7^j.
 * https://oeis.org/A038285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(i, k) * 8^(i-k) * 7^k, where i = the largest
 *            integer with i*(i+1)/2 ≤ n, and k = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_calc(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);      /* multiply numerator */
        r /= (seq_t)i;                /* divide by denominator */
    }
    return r;
}

seq_t a(idx_t n) {
    /* find row index i such that n lies in row i */
    idx_t i   = 0;
    idx_t cum = 0;                     /* cumulative count before row i */
    while (cum + (i + 1) <= n) {        /* while next row would still be ≤ n */
        cum += (i + 1);
        ++i;
    }
    idx_t k = n - cum;                   /* column index within the row */

    seq_t res = binom_calc(i, k);

    /* multiply by 8^(i-k) */
    for (idx_t e = 0; e < i - k; ++e)
        res *= (seq_t)8;

    /* multiply by 7^k */
    for (idx_t e = 0; e < k; ++e)
        res *= (seq_t)7;

    return res;
}
