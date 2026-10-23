/*
 * A167561  The fourth row of the ED2 array A167560.
 * https://oeis.org/A167561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n^3 + 4*n^2 + 16*n (offset = 1)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t before doing any arithmetic */
    seq_t tn = (seq_t)n;
    seq_t tn2 = tn * tn;          /* n^2 */
    seq_t tn3 = tn2 * tn;         /* n^3 */
    return 4 * tn3 + 4 * tn2 + 16 * tn;
}
