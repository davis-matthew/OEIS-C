/*
 * A063811  Erroneous version of A008579.
 * https://oeis.org/A063811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0)=1; a(1)=4; a(2)=6; for n>=3, if n is odd then a(n)=4*n+2,
 *            otherwise a(n)=5*n-2
 * Range:     i8 n <= 16, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)6;

    if (n & 1ULL)  /* odd */
        return ((seq_t)n) * 4 + (seq_t)2;
    else           /* even */
        return ((seq_t)n) * 5 - (seq_t)2;
}
