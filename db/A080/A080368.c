/*
 * A080368  a(n) is the least unitary prime divisor of n, or 0 if no such
 *          prime divisor exists.
 * https://oeis.org/A080368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least unitary prime divisor of n, or 0 if n has no
 *            unitary prime divisor
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    idx_t temp = n;

    /* trial division by all possible prime factors in increasing order */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            unsigned int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp == 1)       /* unitary prime factor found */
                return (seq_t)p;
        }
    }

    /* if a prime > sqrt(original n) remains, its exponent is 1 */
    if (temp > 1)
        return (seq_t)temp;

    /* all prime exponents were >= 2 → n is powerful */
    return 0;
}
