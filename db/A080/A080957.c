/*
 * A080957  Expansion of (5 - 9*x + 6*x^2)/(1-x)^4.
 * https://oeis.org/A080957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^3 + 3*n^2 + 31*n + 30)/6
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1308, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n3 = n2 * nn;      /* n^3 */
    /* Since all terms fit in seq_t for the expected range, ordinary
       integer arithmetic suffices. The division by 6 is exact. */
    return (2 * n3 + 3 * n2 + 31 * nn + 30) / 6;
}
