/*
 * A081196  a(n) = (n+4)^n*binomial(n+2,2).
 * https://oeis.org/A081196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)^n * binomial(n+2, 2) for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(n+2, 2) = (n+2)*(n+1)/2 */
    seq_t binom = ((seq_t)(n + 2) * (seq_t)(n + 1)) / 2;

    /* Compute (n+4)^n by repeated multiplication */
    seq_t base = (seq_t)(n + 4);
    seq_t pow  = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow *= base;
    }

    return pow * binom;
}
