/*
 * A028878  a(n) = (n+3)^2 - 6.
 * https://oeis.org/A028878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)^2 - 6
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (tn + 3) * (tn + 3) - 6;
}
