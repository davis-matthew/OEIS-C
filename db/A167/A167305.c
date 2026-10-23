/*
 * A167305  Totally multiplicative sequence with a(p) = 4*(p+2) for prime p.
 * https://oeis.org/A167305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   * a(n) = ∏_{p^e∥n} (4·(p+2))^e (multiplicative) * where the
 *            product runs over the prime powers p^e dividing n. * * For n = 1
 *            the product is empty, so a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* Factor 2 explicitly for speed and to avoid overflow in p*p */
    if ((temp & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1ULL;
            ++exp;
        }
        if (exp > 0) {
            seq_t factor = (seq_t)4 * (((seq_t)2) + 2);  /* factor = 16 */
            for (uint64_t i = 0; i < exp; ++i)
                result *= factor;
        }
    }

    /* Trial division by odd primes up to sqrt(temp) */
    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p != 0)
            continue;

        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp > 0) {
            seq_t factor = (seq_t)4 * (((seq_t)p) + 2);
            for (uint64_t i = 0; i < exp; ++i)
                result *= factor;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        seq_t factor = (seq_t)4 * (((seq_t)temp) + 2);
        result *= factor;
    }

    return result;
}
