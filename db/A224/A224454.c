/*
 * A224454  The Wiener index of the linear phenylene with n hexagons.
 * https://oeis.org/A224454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2*(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 620, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 as a safe fallback. */
    if (n < 1) {
        return 0;
    }

    seq_t nn = (seq_t)n;          /* Convert index to seq_t */
    seq_t t  = (nn << 1) + 1;     /* 2*n + 1, computed in seq_t to avoid overflow */
    seq_t res = nn * nn;          /* n^2 */
    res *= t;                     /* n^2 * (2*n + 1) */
    res *= 9;                     /* multiply by 9 */

    return res;
}
