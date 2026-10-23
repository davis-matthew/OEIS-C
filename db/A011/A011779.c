/*
 * A011779  Expansion of 1/((1-x)^3*(1-x^3)^2).
 * https://oeis.org/A011779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=0}^{⌊n/3⌋} (m+1) * binom(n - 3m + 2, 2)
 * Range:     u8 n <= 11, u16 n <= 56, u32 n <= 976, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where binom(t,2) = t*(t-1)/2.  This follows from the expansion   */
/* of 1/((1-x)^3*(1-x^3)^2).                                       */
seq_t a(idx_t n)
{
    idx_t m, max_m = n / 3;          /* floor(n/3) */
    seq_t result = 0;

    for (m = 0; m <= max_m; ++m) {
        idx_t diff = n - 3 * m;          /* n - 3m >= 0 */
        seq_t t = (seq_t)diff + 2;        /* t = n - 3m + 2 */

        seq_t binom = t * (t - 1) / 2;    /* C(t,2) */
        seq_t term  = (seq_t)(m + 1) * binom;

        result += term;
    }
    return result;
}
