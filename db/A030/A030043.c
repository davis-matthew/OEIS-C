/*
 * A030043  Unreduced denominator of volume of best symplectic packing of n
 *          balls in 4-dimensional ball.
 * https://oeis.org/A030043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 1 for all n except n = {2,3,5,6,7,8}; a(2) = 2, a(3) = 4,
 *            a(5) = 25, a(6) = 25, a(7) = 64, a(8) = 289
 * Range:     u8 n <= 7, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 2: return (seq_t)2;
        case 3: return (seq_t)4;
        case 5: return (seq_t)25;
        case 6: return (seq_t)25;
        case 7: return (seq_t)64;
        case 8: return (seq_t)289;
        default: return (seq_t)1;   /* n = 1, 4, or n ≥ 9 */
    }
}
