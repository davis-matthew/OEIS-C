/*
 * A051885  Smallest number whose sum of digits is n.
 * https://oeis.org/A051885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   a(n) = ((n mod 9)+1) * 10^floor(n/9) – 1
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 84, u64 n <= 171,
 *            u128 n <= 344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)     /* a(0) = 0 */
        return 0;

    /* Number of full blocks of 9 */
    idx_t groups = n / 9u;
    /* Remainder 0..8 */
    unsigned r = (unsigned)(n % 9u);

    /* Compute 10^groups in seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < groups; ++i)
        pow10 *= (seq_t)10;

    /* ((r+1) * pow10) - 1 as required */
    seq_t result = pow10 * (seq_t)(r + 1u);
    return result - (seq_t)1;
}
