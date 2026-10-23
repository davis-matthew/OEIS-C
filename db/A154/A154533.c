/*
 * A154533  Number of constants of the form a^3*u + b*c*v, where a, b, c are
 *          linear, u of order n-3 and v of order n-2.
 * https://oeis.org/A154533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - 2*n + 10
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result = N * N;     /* N^2 */
    result -= (seq_t)2 * N;    /* -2N */
    result += (seq_t)10;       /* +10 */
    return result;
}
