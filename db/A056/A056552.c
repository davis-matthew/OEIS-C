/*
 * A056552  Powerfree kernel of cubefree part of n.
 * https://oeis.org/A056552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏ p where exponent of prime p in n is NOT a multiple of 3
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e., a(n) equals the product of all distinct primes dividing n whose
   multiplicity in the prime factorization of n is 1 or 2 modulo 3. */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined, but avoid spurious use */
        return 0;

    seq_t result = 1;
    idx_t m = n;

    /* trial division up to sqrt(m) – use p <= m/p to avoid overflow */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp % 3 != 0)            /* exponent not divisible by 3 */
                result *= (seq_t)p;
        }
    }

    /* if a prime factor remains (>1), its exponent is 1 (≠ multiple of 3) */
    if (m > 1)
        result *= (seq_t)m;

    return result;
}
