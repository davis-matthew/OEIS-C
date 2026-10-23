/*
 * A181389  Absolute difference between (sum of previous terms) and (n-th-even
 *          square) with a(1) = 2.
 * https://oeis.org/A181389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*n - 20 for n >= 4 a(1) = 2, a(2) = 2, a(3) = 0 (The
 *            initial values are returned explicitly; for all larger n the
 *            linear form holds.)
 * Range:     u8 n <= 34, u16 n <= 8194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)0;
    /* For n ≥ 4 use the closed form */
    return (seq_t)(8 * n - 20);
}
