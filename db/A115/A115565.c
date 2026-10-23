/*
 * A115565  a(n) = 5*n^4 - 10*n^3 + 20*n^2 - 15*n + 11.
 * https://oeis.org/A115565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^4 - 10*n^3 + 20*n^2 - 15*n + 11
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          // n^2
    seq_t n3 = n2 * nn;          // n^3
    seq_t n4 = n3 * nn;          // n^4
    return 5 * n4 - 10 * n3 + 20 * n2 - 15 * nn + 11;
}
