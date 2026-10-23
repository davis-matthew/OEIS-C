/*
 * A125846  Numerator of volume of best symplectic packing of n balls in
 *          4-dimensional ball.
 * https://oeis.org/A125846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1 for n ≥ 9; otherwise a(1)=1, a(2)=1, a(3)=3, a(4)=1,
 *            a(5)=4, a(6)=24, a(7)=63, a(8)=288
 * Range:     u8 n <= 7, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return 1;
        case 2: return 1;
        case 3: return 3;
        case 4: return 1;
        case 5: return 4;
        case 6: return 24;
        case 7: return 63;
        case 8: return 288;
        default: return 1;
    }
}
