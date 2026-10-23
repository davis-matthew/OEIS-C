/*
 * A129028  A129027(n)/4.
 * https://oeis.org/A129028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5 + 13*n + 7*n^2 + 2*n^3)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sum = 5 + 13*nn + 7*nn*nn + 2*nn*nn*nn;
    return nn * sum;
}
