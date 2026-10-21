/*
 * A060296  Number of regular convex polytopes in n-dimensional space, or -1
 *          if the number is infinite.
 * https://oeis.org/A060296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; a(1)=1; a(2)=-1; a(3)=5; a(4)=6; a(n)=3 for all n>4
 * Range:     i8 n <= 112, i16 n <= 112, i32 n <= 112, i64 n <= 112,
 *            i128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return -1;
    if (n == 3) return 5;
    if (n == 4) return 6;
    return 3;
}
