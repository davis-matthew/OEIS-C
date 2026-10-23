/*
 * A130824  a(n) = 2*A004273(n).
 * https://oeis.org/A130824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 ; a(n) = 4*n - 2 for n > 0 (OEIS A130824)
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    return (seq_t)4 * (seq_t)n - (seq_t)2;
}
