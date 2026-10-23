/*
 * A035184  a(n) = Sum_{d|n} Kronecker(-1, d).
 * https://oeis.org/A035184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-1,d) (multiplicative formula: a(2^e)
 *            = e+1; a(p^e) = e+1 if p ≡ 1 (mod 4); a(p^e) = (1+(-1)^e)/2 if p
 *            ≡ 3 (mod 4))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;

    /* handle factor 2 */
    idx_t e = 0;
    while ((m & 1ULL) == 0) {
        ++e;
        m >>= 1;
    }
    if (e > 0)
        result *= (seq_t)(e + 1);

    /* factor odd primes */
    for (idx_t i = 3; i <= m / i && m > 1; i += 2) {
        if (m % i == 0) {
            idx_t exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            seq_t val;
            if (i % 4 == 1) {
                val = (seq_t)(exp + 1);
            } else {             /* i % 4 == 3 */
                val = (exp % 2 == 0) ? (seq_t)1 : (seq_t)0;
            }
            result *= val;
            if (result == 0)
                return 0;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        if (m % 4 == 1) {
            result *= (seq_t)2;      /* exponent = 1 -> e+1 = 2 */
        } else {                    /* m % 4 == 3 */
            result = 0;
            return 0;
        }
    }

    return result;
}
