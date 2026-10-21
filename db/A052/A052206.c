/*
 * A052206  Partial sums of A050405.
 * https://oeis.org/A052206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n + 7)*C(n+6, 6)/7
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1  (partial sums of A050405) */

seq_t a(idx_t n)
{
    /* Convert index to sequence type for safe arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute C(n+6, 6) multiplicatively to keep intermediate values small. */
    seq_t binom = 1;
    for (int i = 1; i <= 6; ++i) {
        /* binom = binom * (n + 6 - (6 - i)) / i */
        binom = binom * (nn + 6 - (6 - i));
        binom = binom / i;
    }

    /* Compute (9*n + 7) * binom / 7 */
    seq_t coeff = 9 * nn + 7;
    seq_t term  = binom * coeff;
    term        = term / 7;

    return term;
}
