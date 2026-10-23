/*
 * A154033  Number of planar triangular n X n X n nonnegative integer grids
 *          with every similarly oriented 2 X 2 X 2 subtriangle summing to 3.
 * https://oeis.org/A154033
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=10, a(3)=23, a(4)=31, a(5)=35, a(n)=28-(-1)^n for n≥6
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values for the initial terms */
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)31;
    if (n == 5) return (seq_t)35;
    /* n ≥ 6: 27 for even n, 29 for odd n */
    int sign = ((n & (idx_t)1) ? -1 : 1);          /* even → 1, odd → -1 */
    return (seq_t)28 - (seq_t)sign;
}
