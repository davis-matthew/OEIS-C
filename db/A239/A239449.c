/*
 * A239449  a(n) = 7*n^2 - 5*n + 1.
 * https://oeis.org/A239449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2 - 5*n + 1
 * Range:     u8 n <= 6, u16 n <= 97, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = (seq_t)7 * nn * nn;
    result -= (seq_t)5 * nn;
    result += (seq_t)1;
    return result;
}
