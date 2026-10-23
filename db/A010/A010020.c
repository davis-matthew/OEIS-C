/*
 * A010020  a(0) = 1, a(n) = 31*n^2 + 2 for n>0.
 * https://oeis.org/A010020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*n^2 + 2 for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t sn = (seq_t)n;
    seq_t square = sn * sn;       /* n^2 */
    return (seq_t)31 * square + (seq_t)2;
}
