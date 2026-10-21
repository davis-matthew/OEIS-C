/*
 * A227484  The Wiener index of the dendrimer D_4[n], defined pictorially in
 *          the A. R. Ashrafi et al. reference.
 * https://oeis.org/A227484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -74142 + 2^n*(241658-45756*n)+4^n*(115320*n-167090)
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2_n = (seq_t)1;
    seq_t pow4_n = (seq_t)1;

    for (idx_t i = 0; i < n; ++i) {
        pow2_n *= (seq_t)2;
        pow4_n *= (seq_t)4;
    }

    /* compute 4^n * (115320*n)  */
    seq_t term = (seq_t)0;
    if (n) {
        seq_t mul = (seq_t)115320 * (seq_t)n;
        term = pow4_n * mul;
    }

    /* add 2^n * 241658  */
    term += pow2_n * (seq_t)241658;

    /* subtract 2^n * 45756 * n  */
    seq_t sub1 = pow2_n * (seq_t)45756 * (seq_t)n;
    term -= sub1;

    /* subtract 4^n * 167090  */
    seq_t sub2 = pow4_n * (seq_t)167090;
    term -= sub2;

    /* subtract the constant -74142  */
    term -= (seq_t)74142;

    return term;
}
