/*
 * A130773  a(0) = 0, a(1) = 2, a(n) = 2*n + 1 for n >= 2.
 * https://oeis.org/A130773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 2, a(n) = 2*n + 1 for n >= 2
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    return (seq_t)2 * (seq_t)n + (seq_t)1;
}
