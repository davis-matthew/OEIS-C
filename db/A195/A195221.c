/*
 * A195221  Number of lower triangles of a 3 X 3 integer array with each
 *          element differing from all of its diagonal, vertical, antidiagonal
 *          and horizontal neighbors by n or less and triangles differing by a
 *          constant counted only once.
 * https://oeis.org/A195221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (301/30)*n^5 + (301/12)*n^4 + (88/3)*n^3 + (227/12)*n^2 +
 *            (199/30)*n + 1 (Ehrhart polynomial, empirical) Implemented using
 *            a Horner‐style evaluation with integer arithmetic. The
 *            computation is safe for 64‑bit builds up to the largest n whose
 *            result fits in 64 bits; for larger n (or arbitra...
 * Range:     u8 none, u16 n <= 2, u32 n <= 22, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) not needed, but guard for completeness */

    seq_t sn = (seq_t)n;

    /* Horner evaluation of numerator:
       N(n) = 602*n^5 + 1505*n^4 + 1760*n^3 + 1135*n^2 + 398*n + 60
       a(n) = N(n) / 60
    */
    seq_t t0 = sn * (seq_t)602 + (seq_t)1505;  /* t0 = 602*n + 1505          */
    seq_t t1 = t0 * sn + (seq_t)1760;          /* t1 = t0*n + 1760           */
    seq_t t2 = t1 * sn + (seq_t)1135;          /* t2 = t1*n + 1135           */
    seq_t t3 = t2 * sn + (seq_t)398;           /* t3 = t2*n + 398            */

    /* Compute (t3 * sn + 60) / 60 without overflow:
       Write sn = q*60 + r, 0 <= r < 60.
       Then (t3*sn + 60)/60 = t3*q + (t3*r)/60 + 1
    */
    seq_t q = sn / 60;
    seq_t r = sn % 60;

    seq_t part1 = t3 * q;          /* t3*q  (fits for n where result fits 64 bits) */
    seq_t part2 = t3 * r;          /* t3*r  < 60* t3  */
    seq_t ans   = part1 + (part2 / 60) + (seq_t)1;

    return ans;
}
