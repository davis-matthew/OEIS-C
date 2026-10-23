/*
 * A134450  a(n) = square root of the square part of discriminant of Brioschi
 *          quintic polynomial x^5-10*n*x^3+45*n^2*x-n^2.
 * https://oeis.org/A134450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^4*(1728*n - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* Offset starts at 1; behaviour below is irrelevant */
    seq_t sn   = (seq_t)n;           /* Convert index to sequence type */
    seq_t n2   = sn * sn;           /* n^2 */
    seq_t n4   = n2 * n2;           /* n^4 */
    seq_t part = 1728 * sn - 1;     /* (1728*n - 1) */
    return 25 * n4 * part;          /* 25*n^4*(1728*n - 1) */
}
