/*
 * A022150  Coordination sequence for root lattice B_8.
 * https://oeis.org/A022150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = (3968*n^7 - 6720*n^6 + 35840*n^5 -
 *            36960*n^4 + 60032*n^3 - 26880*n^2 + 11040*n)/315
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 393, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    /* use a wider integer type for intermediate computations */
    __int128 nn = ( __int128 ) n;
    __int128 t  = 3968;
    t = t * nn - 6720;
    t = t * nn + 35840;
    t = t * nn - 36960;
    t = t * nn + 60032;
    t = t * nn - 26880;
    t = t * nn + 11040;
    t = t * nn;        /* multiply by n one more time */
    t = t / 315;       /* final division, guaranteed to be exact */
    return (seq_t) t;
}
