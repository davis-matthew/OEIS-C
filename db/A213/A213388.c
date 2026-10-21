/*
 * A213388  Number of (w,x,y) with all terms in {0,...,n} and 2|w-x| >=
 *          max(w,x,y)-min(w,x,y).
 * https://oeis.org/A213388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^3+30*n^2+28*n+3*(((-1)^n)+3))/12
 * Range:     u8 n <= 2, u16 n <= 18, u32 n <= 846, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to the seq_t type for arithmetic */
    seq_t nn = (seq_t)n;

    /* compute numerator: 8*n^3 + 30*n^2 + 28*n + (n even ? 12 : 6) */
    seq_t num = 8 * nn * nn * nn
              + 30 * nn * nn
              + 28 * nn
              + ((n & 1) ? (seq_t)6 : (seq_t)12);

    /* divide by 12 */
    return num / (seq_t)12;
}
