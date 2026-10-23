/*
 * A045506  Inscribe 2 spheres of curvature 2 inside sphere of curvature -1,
 *          continue to inscribe spheres where possible; sequence gives list
 *          of curvatures.
 * https://oeis.org/A045506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = -1 if n==0; otherwise a(n) = 2 + 3*floor((n-1)/2) + (n
 *            even?1:0)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)-1;

    idx_t half = (n - 1) / 2;         /* floor((n-1)/2) */
    seq_t val = (seq_t)2;
    val += (seq_t)3 * (seq_t)half;    /* 3*floor((n-1)/2) */
    if ((n & 1) == 0)                /* check even n */
        val += (seq_t)1;             /* +1 if n even */

    return val;
}
