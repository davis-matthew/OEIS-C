/*
 * A093478  Number of regular (finite but not necessarily convex) polytopes of
 *          full rank in n-dimensional space, or -1 if the number is infinite.
 * https://oeis.org/A093478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = * 1 if n = 0 or 1 * -1 if n = 2 (infinite) * 18 if n = 3
 *            * 34 if n = 4 * 6 otherwise (n ≥ 5)
 * Range:     i8 n <= 112, i16 n <= 112, i32 n <= 112, i64 n <= 112,
 *            i128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    if (n == 2) return -1;
    if (n == 3) return 18;
    if (n == 4) return 34;
    return 6;
}
