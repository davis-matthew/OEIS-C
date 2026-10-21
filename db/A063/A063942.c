/*
 * A063942  Follow k with k-1 and k-2.
 * https://oeis.org/A063942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n/3) + { 1, 0, -1 }[n%3] (a(3k)=k+1, a(3k+1)=k,
 *            a(3k+2)=k-1)
 * Range:     i8 n <= 380, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;          /* integer division, floor(n/3) */
    idx_t r = n - q * 3;      /* remainder: 0,1,or2 */

    seq_t v = (seq_t)q;       /* cast once to signed type */
    if (r == 0) return v + 1; /* a(3k)   = k + 1 */
    if (r == 1) return v;     /* a(3k+1) = k   */
    /* r == 2 */               /* a(3k+2) = k - 1 */
    return v - 1;
}
