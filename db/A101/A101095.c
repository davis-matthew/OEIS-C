/*
 * A101095  Fourth difference of fifth powers (A000584).
 * https://oeis.org/A101095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 120*(n-2) for n >= 4, with a(1) = 1, a(2) = 28, a(3) =
 *            121
 * Range:     u8 n <= 4, u16 n <= 548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 28;
    if (n == 3) return 121;
    return (seq_t)n * (seq_t)120 - (seq_t)240;
}
