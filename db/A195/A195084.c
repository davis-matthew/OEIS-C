/*
 * A195084  a(2n-1) = 2-n, a(2n) = 2+n.
 * https://oeis.org/A195084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((2*n+1)*(-1)^n + 7) / 4
 * Range:     i8 n <= 59, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative form: a(2k) = 2 + k,   a(2k-1) = 2 - k  (k ≥ 1)        */

seq_t a(idx_t n)
{
    /* Determine (-1)^n as +1 for even n, -1 for odd n */
    seq_t sign = ( (n & 1) == 0 ) ? (seq_t)1 : (seq_t)-1;

    /* Compute 2*n as seq_t to avoid unsigned promotion issues */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* Numerator: (2*n + 1) * sign + 7   ; divisible by 4 */
    seq_t numerator = (two_n + 1) * sign + 7;

    /* Final division gives the term value */
    return numerator / 4;
}
