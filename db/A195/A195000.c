/*
 * A195000  Number of lower triangles of a 4 X 4 0..n array with each element
 *          differing from all of its horizontal and vertical neighbors by
 *          one.
 * https://oeis.org/A195000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 216*n - 408 for n>4; a(1)=2, a(2)=80, a(3)=248, a(4)=458
 * Range:     u8 n <= 3, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;
    if (n == 2) return 80;
    if (n == 3) return 248;
    if (n == 4) return 458;
    return ((seq_t)216 * (seq_t)n) - 408;
}
