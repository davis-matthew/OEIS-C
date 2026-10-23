/*
 * A124473  y-coordinate of position of n in the magic square in A124471.
 * https://oeis.org/A124473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = [8, 7, 2, 1, 3, 4, 5, 6, 6, 5, 4, 3, 1, 2, 7, 8][(n-1)
 *            mod 16]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t t[8] = {8, 7, 2, 1, 3, 4, 5, 6};
    if (n == 0)
        return 0;            /* undefined for n < 1, return 0 */
    idx_t i = (n - 1) & 15u;  /* (n-1) mod 16, 16 is power of two */
    if (i < 8)
        return t[i];
    else
        return t[15 - i];
}
