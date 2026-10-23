/*
 * A025802  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^5)).
 * https://oeis.org/A025802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(11+5*(-1)^n) + 53 + 27*(-1)^n)/80)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Even n: (-1)^n = 1 => numerator = n^2 + 16*n + 80
       Odd  n: (-1)^n = -1 => numerator = n^2 + 6*n + 26   */
    if ((n & 1ULL) == 0) {
        return (N * N + 16 * N + 80) / 80;
    } else {
        return (N * N + 6 * N + 26) / 80;
    }
}
