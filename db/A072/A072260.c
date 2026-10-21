/*
 * A072260  a(n) = ((6*n+19)*4^n - 1)/3.
 * https://oeis.org/A072260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n + 19) * 4^n - 1) / 3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* compute the coefficient (6*n + 19) */
    seq_t coeff = (seq_t)6 * n_seq + 19;

    /* compute 4^n using binary exponentiation */
    seq_t pow4 = 1;
    seq_t base  = 4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1)
            pow4 = pow4 * base;
        if (exp > 1)   /* avoid one unnecessary multiplication of very large base */
            base = base * base;
        exp >>= 1;
    }

    /* apply the formula: ((coeff * pow4) - 1) / 3 */
    seq_t numerator = coeff * pow4 - 1;
    seq_t result    = numerator / 3;

    return result;
}
