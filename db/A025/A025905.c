/*
 * A025905  Expansion of 1/((1-x^6)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A025905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+4*n+212)/1188 + (n+5)*((n+2) mod 3)/54 +
 *            ((6*n^2+2*n+7) mod 11)/11)
 * Range:     u8 n <= 2, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* convert index to the sequence type */
    seq_t nseq = (seq_t)n;

    /* term1: (n^2 + 4*n + 212) */
    seq_t term1 = nseq * nseq + 4 * nseq + 212;

    /* term2: 22 * (n+5) * ((n+2) mod 3) */
    seq_t mod3 = (nseq + 2) % (seq_t)3;
    seq_t term2 = 22 * (nseq + 5) * mod3;

    /* compute ((6*n^2 + 2*n + 7) mod 11) without overflow */
    seq_t nmod11      = nseq % (seq_t)11;
    seq_t tmp         = ((seq_t)6 * nmod11) % (seq_t)11;
    tmp               = (tmp * nmod11) % (seq_t)11;
    tmp               = (tmp + ((seq_t)2 * nmod11) % (seq_t)11) % (seq_t)11;
    tmp               = (tmp + (seq_t)7) % (seq_t)11;
    seq_t term3       = 108 * tmp;          /* 108 = 1188 / 11 */

    /* total numerator */
    seq_t num = term1 + term2 + term3;

    /* floor division by 1188 */
    return num / (seq_t)1188;
}
