/*
 * A151255  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, -1), (-1, 1), (1,
 *          0)}.
 * https://oeis.org/A151255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{l=0}^{⌊n/2⌋} C(n,l) * C(l,⌊l/2⌋) * (n-2l+1)/(n-l+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t max_l = n / 2;
    seq_t binom_n_l = 1;   /* C(n,0)   */
    seq_t binom_mid = 1;   /* C(0,0)   */

    for (idx_t l = 0; l <= max_l; ++l)
    {
        seq_t num = (seq_t)(n - 2 * l + 1); /* numerator part   */
        seq_t den = (seq_t)(n - l + 1);     /* denominator part */

        /* term = C(n,l) * C(l,⌊l/2⌋) * num / den  */
        seq_t term = binom_n_l * binom_mid;
        term = term * num;
        term = term / den;
        result += term;

        if (l == max_l) break;

        /* update C(n,l+1) = C(n,l) * (n-l)/(l+1) */
        binom_n_l = binom_n_l * (seq_t)(n - l);
        binom_n_l = binom_n_l / (seq_t)(l + 1);

        /* update C(l+1,⌊(l+1)/2⌋) = C(l,⌊l/2⌋) * (l+1)/(⌊l/2⌋+1) */
        idx_t mid = l / 2;                     /* ⌊l/2⌋ */
        binom_mid = binom_mid * (seq_t)(l + 1);
        binom_mid = binom_mid / (seq_t)(mid + 1);
    }
    return result;
}
