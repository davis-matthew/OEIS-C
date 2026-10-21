/*
 * A219070  a(n) = (46*n^5 + 30*n^4 + 15*n^3 - n) / 30.
 * https://oeis.org/A219070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (46*n^5 + 30*n^4 + 15*n^3 - n) / 30
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                 /* cast to seq_t */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    /* Intermediate polynomial: 46*n^4 + 30*n^3 + 15*n^2 - 1 */
    seq_t poly = 46 * n4 + 30 * n3 + 15 * n2 - (seq_t)1;
    seq_t q = nn / 30;
    seq_t r = nn % 30;
    return q * poly + (r * poly) / 30;
}
