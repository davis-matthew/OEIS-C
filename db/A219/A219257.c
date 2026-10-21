/*
 * A219257  Numbers k such that 11*k+1 is a square.
 * https://oeis.org/A219257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 0 * if n is even: let m = n/2; a(n) = m * (11*m - 2) *
 *            if n is odd (n>1): let m = (n-1)/2; a(n) = m * (11*m + 2)
 * Range:     u8 n <= 9, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    if ((n & 1ULL) == 0) {
        seq_t m = (seq_t)(n >> 1);          /* m = n/2                */
        return m * ((seq_t)11 * m - 2);     /* m*(11*m - 2)           */
    } else {
        seq_t m = (seq_t)((n - 1) >> 1);    /* m = (n-1)/2            */
        return m * ((seq_t)11 * m + 2);     /* m*(11*m + 2)           */
    }
}
