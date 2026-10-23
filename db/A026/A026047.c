/*
 * A026047  a(n) = (d(n)-r(n))/2, where d = A026046 and r is the periodic
 *          sequence with fundamental period (0,1,0,1).
 * https://oeis.org/A026047
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^3 - 6*n^2 + 38*n)/3 - (n mod 2)) / 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n² */
    seq_t n3 = n2 * n_seq;             /* n³ */
    seq_t q = (n3 - (seq_t)6 * n2 + (seq_t)38 * n_seq) / (seq_t)3;
    /* subtract 1 for odd n to make the value integer before division by 2 */
    seq_t res = (q - (n_seq & (seq_t)1)) / (seq_t)2;
    return res;
}
