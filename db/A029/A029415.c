/*
 * A029415  Expansion of 1/((1-x^6)*(1-x^7)*(1-x^8)*(1-x^11)).
 * https://oeis.org/A029415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+48*n^2+816*n+2952)/22176 - (n mod 2)*n/96 +
 *            ((5*n^3+2*n^2+6*n+4) mod 7)/7 + ((7*n^3+6*n^2+3*n+6) mod 11)/11)
 * Range:     u8 none, u16 n <= 20, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t n2 = n_seq * n_seq;        /* n^2 */
    seq_t n3 = n2 * n_seq;           /* n^3 */

    /* main rational part */
    seq_t term1 = n3
                + (seq_t)48 * n2
                + (seq_t)816 * n_seq
                + (seq_t)2952;

    /* -(n mod 2)*n/96  => multiply by 22176/96 = 231 */
    seq_t term2 = ((n & (idx_t)1) * n_seq) * (seq_t)231;

    /* ((5*n^3+2*n^2+6*n+4) mod 7)/7  => multiply remainder by 22176/7 = 3168 */
    seq_t expr3 = (seq_t)5 * n3
                + (seq_t)2 * n2
                + (seq_t)6 * n_seq
                + (seq_t)4;
    seq_t term3 = (expr3 % (seq_t)7) * (seq_t)3168;

    /* ((7*n^3+6*n^2+3*n+6) mod 11)/11 => multiply remainder by 22176/11 = 2016 */
    seq_t expr4 = (seq_t)7 * n3
                + (seq_t)6 * n2
                + (seq_t)3 * n_seq
                + (seq_t)6;
    seq_t term4 = (expr4 % (seq_t)11) * (seq_t)2016;

    /* combine with common denominator 22176 */
    seq_t numer = term1 - term2 + term3 + term4;
    return numer / (seq_t)22176;
}
