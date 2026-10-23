/*
 * A136442  a(3n) = 1, a(3n-1) = 0 and a(3n+1) = a(n).
 * https://oeis.org/A136442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3*n) = 1, a(3*n-1) = 0, a(3*n+1) = a(n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 0) return (seq_t)1;
    if (n % 3 == 2) return (seq_t)0;
    return a((n - 1) / 3);
}
