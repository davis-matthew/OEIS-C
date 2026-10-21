/*
 * A089145  Greatest common divisor of n^2-3 and n^2+3.
 * https://oeis.org/A089145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n+3, 6)
 * Range:     u8 n <= 282, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n+3 as a seq_t value */
    seq_t val = (seq_t)n + (seq_t)3;
    /* modulus 6 is sufficient to determine the gcd with 6 */
    seq_t r = val % (seq_t)6;

    if (r == (seq_t)0)
        return (seq_t)6;
    if (r % (seq_t)3 == (seq_t)0)
        return (seq_t)3;
    if (r % (seq_t)2 == (seq_t)0)
        return (seq_t)2;
    return (seq_t)1;
}
