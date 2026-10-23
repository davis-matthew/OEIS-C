/*
 * A153727  Period 3: repeat [1, 4, 2] ; Trajectory of 3x+1 sequence starting
 *          at 1.
 * https://oeis.org/A153727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n)=1, a(3n+1)=4, a(3n+2)=2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 3) {
    case 0:
        return (seq_t)1;
    case 1:
        return (seq_t)4;
    default:
        return (seq_t)2;
    }
}
