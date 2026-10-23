/*
 * A195318  Centered 44-gonal numbers.
 * https://oeis.org/A195318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*n^2 - 22*n + 1
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 // offset is 1; value below offset is irrelevant
    seq_t nn = (seq_t)n;
    return 22 * nn * nn - 22 * nn + 1;
}
