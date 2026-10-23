/*
 * A174390  a(2n) = -n, a(2n+1) = 2*n+4.
 * https://oeis.org/A174390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (1/4)*(n + 6 - 3*(-1)^n*(n + 2))
 * Range:     i8 n <= 124, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent definition: a(2n) = -n; a(2n+1) = 2*n + 4 */

seq_t a(idx_t n)
{
    /* Even indices: a(2k) = -k */
    if ((n & 1) == 0) {
        seq_t k = (seq_t)(n >> 1);   /* n/2 */
        return -k;
    }
    /* Odd indices: a(2k+1) = 2*k + 4 */
    seq_t k = (seq_t)((n - 1) >> 1); /* (n-1)/2 */
    return ((seq_t)2 * k) + 4;
}
