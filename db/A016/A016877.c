/*
 * A016877  a(n) = (5*n + 2)^5.
 * https://oeis.org/A016877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 16, u64 n <= 1425,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 5*n + 2, using seq_t to avoid overflow on n */
    seq_t base = (seq_t)5 * (seq_t)n + (seq_t)2;

    /* raise base to the 5th power by successive multiplications */
    seq_t result = base * base;   /* base^2 */
    result *= base;               /* base^3 */
    result *= base;               /* base^4 */
    result *= base;               /* base^5 */

    return result;
}
