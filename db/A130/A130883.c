/*
 * A130883  a(n) = 2*n^2 - n + 1.
 * https://oeis.org/A130883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - n + 1
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn * 2) - nn + 1;
}
