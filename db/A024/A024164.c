/*
 * A024164  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that c - b = b - a.
 * https://oeis.org/A024164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n mod 3 ≠ 0; otherwise a(n) = floor((n-3)/6)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n < 3 cannot give a positive result */
    if (n < 3) return 0;
    /* Only multiples of 3 can produce triangles */
    if (n % 3 != 0) return 0;
    /* Result is floor((n-3)/6) */
    return ((seq_t)(n - 3)) / 6;
}
