/*
 * A185437  The least number of colors required to color an n-bead necklace so
 *          that each bead can be identified.
 * https://oeis.org/A185437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(3)=a(4)=a(5)=3; a(n)=2 for all other n>=2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 3 || n == 4 || n == 5) return 3;
    return 2;
}
