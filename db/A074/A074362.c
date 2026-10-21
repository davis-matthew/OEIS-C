/*
 * A074362  Coefficient of q^2 in nu(n), where nu(0) = 1, nu(1) = b and, for n
 *          >= 2, nu(n) = b*nu(n-1) + lambda*(1 + q + q^2 + ... +
 *          q^(n-2))*nu(n-2) with (b,lambda) = (3,1).
 * https://oeis.org/A074362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficient of q^2 in nu(n) where
 * nu(0) = 1, nu(1) = 3,
 * nu(n) = 3*nu(n-1) + (1 + q + … + q^(n-2))*nu(n-2)   (for n ≥ 2)   */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1)          /* no q‑terms in nu(0) or nu(1) */
        return 0;

    /* Previous two values of the three coefficients */
    seq_t c0_n_2 = 1;  /* nu(0) constant term  */
    seq_t c1_n_2 = 0;  /* nu(0) q^1 coeff     */
    seq_t c2_n_2 = 0;  /* nu(0) q^2 coeff     */

    seq_t c0_n_1 = 3;  /* nu(1) constant term  */
    seq_t c1_n_1 = 0;  /* nu(1) q^1 coeff     */
    seq_t c2_n_1 = 0;  /* nu(1) q^2 coeff     */

    for (idx_t i = 2; i <= n; ++i) {
        /* n-2 = i-2  */
        idx_t m = i - 2;

        /* product contribution to q^2 */
        seq_t prod_c2 = c2_n_2;
        if (m >= 1) prod_c2 += c1_n_2;
        if (m >= 2) prod_c2 += c0_n_2;

        /* product contribution to q^1 */
        seq_t prod_c1 = c1_n_2;
        if (m >= 1) prod_c1 += c0_n_2;

        /**** new coefficients for nu(i) ****/
        seq_t new_c0 = (seq_t)3 * c0_n_1 + c0_n_2;
        seq_t new_c1 = (seq_t)3 * c1_n_1 + prod_c1;
        seq_t new_c2 = (seq_t)3 * c2_n_1 + prod_c2;

        /* shift for next iteration */
        c0_n_2 = c0_n_1;   c0_n_1 = new_c0;
        c1_n_2 = c1_n_1;   c1_n_1 = new_c1;
        c2_n_2 = c2_n_1;   c2_n_1 = new_c2;
    }

    return c2_n_1;  /* coefficient of q^2 in nu(n) */
}
