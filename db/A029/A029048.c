/*
 * A029048  Expansion of 1/((1-x)*(1-x^3)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 63*n^2 + 534*n + 2316)/2376 +
 *            ((n+9)/18)*[(n mod 3)=0])
 * Range:     u8 none, u16 n <= 21, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;
    /* Compute the rational numerator with common denominator 2376 */
    /* term1 = 2*n^3 + 63*n^2 + 534*n + 2316 */
    seq_t term1 = 2*N*N*N + 63*N*N + 534*N + 2316;
    /* term2 = indicator * (n+9) * (2376/18) = indicator * (n+9) * 132 */
    seq_t term2 = (n % 3 == 0) ? (N + 9) * 132 : 0;
    /* total numerator */
    seq_t total = term1 + term2;
    /* Divide by 2376 to obtain the floor value */
    seq_t result = total / 2376;
    return result;
}
