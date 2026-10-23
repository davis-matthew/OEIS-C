/*
 * A167586  The fourth row of the ED4 array A167584.
 * https://oeis.org/A167586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 32*n^3 - 16*n^2 + 120*n - 60
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 512, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t val = ((seq_t)32) * n3;
    val -= ((seq_t)16) * n2;
    val += ((seq_t)120) * nn;
    val -= (seq_t)60;
    return val;
}
