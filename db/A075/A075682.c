/*
 * A075682  First differences of A075681.
 * https://oeis.org/A075682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(3*n^2 + 17*n - 38) for n > 3; * a(1) = 0, a(2) =
 *            2, a(3) = 19
 * Range:     u8 n <= 7, u16 n <= 205, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms specially. */
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 19;

    /* Compute the formula using only seq_t arithmetic. */
    seq_t N = (seq_t)n;                /* cast the unsigned index to seq_t */
    seq_t val = 3 * N * N + 17 * N - 38;
    val /= 2;
    return val;
}
