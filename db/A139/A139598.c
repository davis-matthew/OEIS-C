/*
 * A139598  A035008(n) followed by A139098(n+1).
 * https://oeis.org/A139598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = 1 - (-1)^n + 4*n + 2*n^2
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t val = 2 * tn * tn + 4 * tn;
    if (n & (idx_t)1) {
        val += 2;
    }
    return val;
}
