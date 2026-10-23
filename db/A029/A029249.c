/*
 * A029249  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^5)*(1-x^11)).
 * https://oeis.org/A029249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((262*n^3+399*n^2+588*n+720)/720) -
 *            floor((4*n^3+6*n^2+8*n+6)/11)
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 341, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Precompute powers to avoid recomputation */
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;

    /* First term: floor((262*n^3 + 399*n^2 + 588*n + 720) / 720) */
    seq_t term1 = ((seq_t)262 * nn3
                   + (seq_t)399 * nn2
                   + (seq_t)588 * nn
                   + (seq_t)720) / (seq_t)720;

    /* Second term: floor((4*n^3 + 6*n^2 + 8*n + 6) / 11) */
    seq_t term2 = ((seq_t)4 * nn3
                   + (seq_t)6 * nn2
                   + (seq_t)8 * nn
                   + (seq_t)6) / (seq_t)11;

    return term1 - term2;
}
