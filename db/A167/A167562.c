/*
 * A167562  The fifth row of the ED2 array A167560.
 * https://oeis.org/A167562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n^4 + 10*n^3 + 67*n^2 + 14*n + 24
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 170, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset: 1 (a(1) = 120) */
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t val = 5 * x4 + 10 * x3 + 67 * x2 + 14 * x + 24;
    return val;
}
