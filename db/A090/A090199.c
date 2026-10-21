/*
 * A090199  a(n) = N(6,n), where N(6,x) is the 6th Narayana polynomial.
 * https://oeis.org/A090199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + 15*n^4 + 50*n^3 + 50*n^2 + 15*n + 1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 81, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)1;          /* start with constant term 1 */
    res = res * sn + (seq_t)15;    /* +15*n */
    res = res * sn + (seq_t)50;    /* +50*n^2 */
    res = res * sn + (seq_t)50;    /* +50*n^3 */
    res = res * sn + (seq_t)15;    /* +15*n^4 */
    res = res * sn + 1;            /* +n^5 */
    return res;
}
