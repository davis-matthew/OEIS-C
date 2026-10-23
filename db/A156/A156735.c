/*
 * A156735  a(n) = 57122*n^2 + 47320*n + 9801.
 * https://oeis.org/A156735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 57122*n^2 + 47320*n + 9801
 * Range:     u8 none, u16 n <= 0, u32 n <= 273, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t tnsq = tn * tn;
    seq_t res  = 57122ULL * tnsq + 47320ULL * tn + 9801ULL;
    return res;
}
