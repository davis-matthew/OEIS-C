/*
 * A010001  a(0) = 1, a(n) = 5*n^2 + 2 for n>0.
 * https://oeis.org/A010001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 + 2 for n>0, a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
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
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* n*n in seq_t */
    seq_t res  = (seq_t)5 * sq + (seq_t)2;
    return res;
}
