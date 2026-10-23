/*
 * A041222  Numerators of continued fraction convergents to sqrt(123).
 * https://oeis.org/A041222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 11; for n ≥ 1, * a(n) = c_n * a(n‑1) + a(n‑2), * where
 *            c_n = 11 if n is odd, otherwise 22. * This recurrence generates
 *            the numerators of the * continued fraction convergents of
 *            sqrt(123)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t h_prev2 = 0;   /* a_{-2} */
    seq_t h_prev1 = 1;   /* a_{-1} */
    seq_t h_curr;

    for (i = 0; i <= n; ++i) {
        seq_t coeff = ((i == 0) || ((i & 1) == 1)) ? (seq_t)11 : (seq_t)22;
        h_curr = coeff * h_prev1 + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }
    return h_curr;
}
