/*
 * A154032  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          2 X 2 X 2 subtriangle summing to 3.
 * https://oeis.org/A154032
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2)=1, a(3)=2, a(4)=10; for n >= 5 the terms repeat with period
 *            6: a(n) = {2,3,11,3,2,12}[ (n-5) mod 6 ]
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2U) return (seq_t)1;
    if (n == 3U) return (seq_t)2;
    if (n == 4U) return (seq_t)10;
    /* n >= 5 */
    idx_t t = (n - 5U) % 6U;
    switch (t) {
        case 0U: return (seq_t)2;
        case 1U: return (seq_t)3;
        case 2U: return (seq_t)11;
        case 3U: return (seq_t)3;
        case 4U: return (seq_t)2;
        default: /* case 5U */ return (seq_t)12;
    }
}
