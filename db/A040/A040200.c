/*
 * A040200  Continued fraction for sqrt(215).
 * https://oeis.org/A040200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   * a(0) = 14 * a(n) = 1 if n is odd or n == 2 * a(n) = 28 if n is
 *            a power of 2 with exponent ≥ 2 * (This matches the
 *            multiplicative definition: * a(2) = 1, a(2^e) = 28 for e ≥ 2,
 *            a(p^e) = 1 for odd primes p.)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)14;

    /* Count the exponent of 2 in n */
    idx_t e = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        ++e;
        m >>= 1;
    }

    /* Decide the value based on the exponent of 2 */
    if (e == 0 || e == 1)    /* odd numbers or n == 2 */
        return (seq_t)1;
    else                     /* powers of 2 with exponent ≥ 2 */
        return (seq_t)28;
}
