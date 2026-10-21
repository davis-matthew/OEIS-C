/*
 * A201554  Number of arrays of 7 integers in -n..n with sum zero.
 * https://oeis.org/A201554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 7*n*(n+1)*(841*n^4+1682*n^3+1568*n^2+727*n+222)/180
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 22, u64 n <= 908,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote index to a wider type for intermediate arithmetic */
    unsigned __int128 N = n;

    /* Horner evaluation of the polynomial 841*n^4 + 1682*n^3 + 1568*n^2 + 727*n + 222 */
    unsigned __int128 poly = 841;
    poly = poly * N + 1682;
    poly = poly * N + 1568;
    poly = poly * N + 727;
    poly = poly * N + 222;

    /* Compute 7*n*(n+1)*poly / 180   (all in 128‑bit to avoid overflow) */
    unsigned __int128 tmp = 7 * N * (N + 1) * poly;
    tmp /= 180;

    /* Add the constant term and cast back to seq_t */
    return (seq_t)(1 + tmp);
}
