/*
 * A048072  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(floor((n+2)/3), floor((n -
 *            floor((n+2)/3))/2))} * binom(floor((n+2)/3), k) * binom(n -
 *            floor((n+2)/3), 2k)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = floor((n+2)/3);  u = n - t;  limit = min(t, u/2) */
    idx_t t = (n + 2) / 3;
    idx_t u = n - t;
    idx_t half_u = u / 2;
    idx_t limit = (t < half_u) ? t : half_u;

    seq_t sum = 0;
    seq_t C_tk = 1;   /* binom(t, 0) */
    seq_t C_um = 1;   /* binom(u, 0) */

    for (idx_t k = 1; k <= limit; ++k) {
        /* update C(t, k) */
        C_tk = C_tk * (seq_t)(t - k + 1) / (seq_t)k;

        /* update C(u, 2k) from C(u, 2(k-1)) */
        /* first step: m = 2(k-1) -> m+1 = 2k-1 */
        C_um = C_um * (seq_t)(u - (2 * (k - 1))) / (seq_t)((2 * k) - 1);
        /* second step: m+1 -> m+2 = 2k */
        C_um = C_um * (seq_t)(u - ((2 * k) - 1)) / (seq_t)(2 * k);

        sum += C_tk * C_um;
    }
    return sum;
}
