/*
 * A212679  Number of (w,x,y,z) with all terms in {1,...,n} and |x-y|=|y-z|.
 * https://oeis.org/A212679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n^2 - n + floor(n/2)^2 + ceil(n/2)^2) This counts
 *            the number of quadruples (w,x,y,z) with all terms in {1,…,n}
 *            satisfying |x‑y| = |y‑z|. The derivation uses the fact that the
 *            condition holds either when x = z (n^2 possibilities) or when
 *            x+z = 2y (number of even‑parity pairs). After re...
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* cast index to seq_t */
    seq_t E   = ns / 2;           /* number of even terms in [1,n] */
    seq_t O   = ns - E;           /* number of odd terms in [1,n] */
    seq_t ss  = E * E + O * O;    /* E^2 + O^2  (number of same–parity pairs) */
    seq_t T   = ns * ns - ns + ss;/* number of triples (x,y,z) */
    return ns * T;                /* multiply by the n choices for w */
}
