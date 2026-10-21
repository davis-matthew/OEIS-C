/*
 * A064305  Ninth diagonal of triangle A064094.
 * https://oeis.org/A064305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 7*n + 27*n^2 + 75*n^3 + 165*n^4 + 297*n^5 + 429*n^6 +
 *            429*n^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 237, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* convert index to seq_t type */
    seq_t r  = 429;             /* start with leading coefficient a7 */
    r = r * nn + 429;           /* a6 coefficient */
    r = r * nn + 297;           /* a5 coefficient */
    r = r * nn + 165;           /* a4 coefficient */
    r = r * nn + 75;            /* a3 coefficient */
    r = r * nn + 27;            /* a2 coefficient */
    r = r * nn + 7;             /* a1 coefficient */
    r = r * nn + 1;             /* a0 coefficient (constant term) */
    return r;
}
