/*
 * A029280  Expansion of 1/((1-x^3)*(1-x^5)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A029280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((215*n^3+69*n^2+228*n+216)/198) -
 *            floor((4*n^3+n^2+4*n+2)/5) - floor((2*n^3+n^2+2*n+4)/7)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts 3, 5, 7, and 11. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;              /* n as seq_t */
    seq_t n2 = sn * sn;               /* n^2 */
    seq_t n3 = n2 * sn;               /* n^3 */

    seq_t term1 = ((seq_t)215 * n3 + (seq_t)69 * n2 + (seq_t)228 * sn + (seq_t)216) / (seq_t)198;
    seq_t term2 = ((seq_t)4 * n3 + n2 + (seq_t)4 * sn + (seq_t)2) / (seq_t)5;
    seq_t term3 = ((seq_t)2 * n3 + n2 + (seq_t)2 * sn + (seq_t)4) / (seq_t)7;

    return term1 - term2 - term3;
}
