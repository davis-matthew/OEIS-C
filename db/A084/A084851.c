/*
 * A084851  Binomial transform of binomial(n+2,2).
 * https://oeis.org/A084851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 7*n + 8) * 2^(n - 3) for n >= 0
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial transform of binomial(n+2,2) */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    const seq_t nn = (seq_t)n;
    /* Compute (n^2 + 7*n + 8) */
    seq_t coeff = nn * nn + (seq_t)7 * nn + (seq_t)8;

    if (n >= 3) {
        /* Multiply by 2^(n-3) */
        seq_t pow2 = ((seq_t)1) << (unsigned)(n - 3);
        return coeff * pow2;
    } else {
        /* n < 3 : divide by 2^(3-n) */
        seq_t pow2 = ((seq_t)1) << (unsigned)(3 - n);
        return coeff / pow2;
    }
}
