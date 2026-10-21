/*
 * A099136  Inverse integer permutation to A099135.
 * https://oeis.org/A099136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n for n <= 1 or n >= 15; a(2)=12, a(3)=2, a(4)=8, a(5)=9,
 *            a(6)=3, a(7)=13, a(8)=4, a(9)=14, a(10)=5, a(11)=10, a(12)=7,
 *            a(13)=6, a(14)=11
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;
    if (n <= 14) {
        /* index 0 unused, 1..14 contained */
        static const seq_t m[15] = {
            0,  /* dummy for index 0 */
            1, 12, 2, 8, 9, 3, 13, 4, 14, 5, 10, 7, 6, 11
        };
        return m[n];
    }
    return (seq_t)n;
}
