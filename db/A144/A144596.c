/*
 * A144596  Christoffel word of slope 2/7.
 * https://oeis.org/A144596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) (period 9). Initial values: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=0, a(8)=1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[9] = {0, 0, 0, 0, 1, 0, 0, 0, 1};
    return pattern[n % 9];
}
