/*
 * A099894  XOR BINOMIAL transform of A038712.
 * https://oeis.org/A099894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1 if n+1 is a power of two, else a(n) = 0. * This
 *            follows from a(2^k-1)=2^k for k≥0 and a(n)=0 otherwise
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;        /* m = n + 1, might wrap around */
    seq_t res = 0;
    if (m != 0 && (m & (m - 1)) == 0) {
        /* m is a power of two → a(n) = m = n+1 */
        res = m;
    }
    return res;
}
