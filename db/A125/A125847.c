/*
 * A125847  Denominator of volume of best symplectic packing of n balls in
 *          4-dimensional ball.
 * https://oeis.org/A125847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator of best symplectic packing density:
   a(1)=1, a(2)=2, a(3)=4, a(4)=1, a(5)=5, a(6)=25,
   a(7)=64, a(8)=289; for all n≥9 the value is 1 (Biran’s theorem). */

seq_t a(idx_t n)
{
    switch (n) {
    case 1: return (seq_t)1;
    case 2: return (seq_t)2;
    case 3: return (seq_t)4;
    case 4: return (seq_t)1;
    case 5: return (seq_t)5;
    case 6: return (seq_t)25;
    case 7: return (seq_t)64;
    case 8: return (seq_t)289;
    default: return (seq_t)1;
    }
}
