/*
 * A097062  Interleave 2*n+1 and 2*n-1.
 * https://oeis.org/A097062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n-1)/2 + 3*(-1)^n/2 (equivalently a(n) = n+1 for even
 *            n, n-2 for odd n)
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;               /* convert index to signed seq_t */
    if ((n & 1) == 0)                   /* even index */
        return tn + 1;                  /* a(2k) = 2k+1 = n+1 */
    else                                /* odd index */
        return tn - 2;                  /* a(2k+1) = 2k-1 = n-2 */
}
