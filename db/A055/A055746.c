/*
 * A055746  Product of first n terms of A003046.
 * https://oeis.org/A055746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} (∏_{j=1}^{k} Catalan(j)) , where Catalan(j) =
 *            binom(2*j, j)/(j+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result   = (seq_t)1;  /* a(0) = 1 */
    seq_t cur_prod = (seq_t)1;  /* product of Catalan numbers up to current k */
    seq_t catalan  = (seq_t)1;  /* C_0 = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute C_i from C_{i-1}:
           C_i = C_{i-1} * 2*(2*(i-1)+1) / (i+1)   (exact division) */
        seq_t factor = (seq_t)(2ULL * (2ULL * (i - 1) + 1ULL));
        catalan = (catalan * factor) / (seq_t)(i + 1);

        /* update running product of Catalan numbers and final result */
        cur_prod *= catalan;
        result   *= cur_prod;
    }
    return result;
}
