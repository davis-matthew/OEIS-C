/*
 * A166625  Totally multiplicative sequence with a(p) = 4p for prime p.
 * https://oeis.org/A166625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 4^Ω(n) where Ω(n) = sum of prime exponents of n
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;               /* a(1) = 1 */

    idx_t temp = n;
    idx_t bigomega = 0;         /* total exponent count */

    /* Handle factor 2 separately */
    while ((temp & 1) == 0) {   /* temp % 2 == 0 */
        temp >>= 1;             /* temp /= 2 */
        ++bigomega;
    }

    /* Handle odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        while (temp % p == 0) {
            temp /= p;
            ++bigomega;
        }
    }

    /* If one prime factor remains >1 */
    if (temp > 1)
        ++bigomega;

    /* Compute 4^bigomega as seq_t */
    seq_t pow = 1;
    for (idx_t i = 0; i < bigomega; ++i)
        pow *= 4;

    /* Return n * 4^bigomega */
    return (seq_t)n * pow;
}
