/*
 * A162419  a(n) = sigma(n)*|A002129(n)| where sigma(n) = A000203(n).
 * https://oeis.org/A162419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   * a(n) = multiplicative function with * a(2^e) =
 *            (2^(e+1)-1)*(2^(e+1)-3) for e≥1, a(1)=1; * a(p^e) =
 *            ((p^(e+1)-1)/(p-1))^2 for odd prime p. * Equivalently, a(odd n)
 *            = sigma(n)^2; a(even n) = a(2^v2(n)) * sigma(odd_part)^2
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute σ(p^e) = 1 + p + p^2 + … + p^e */
static seq_t sigma_prime_power(uint64_t p, uint64_t e)
{
    seq_t sum = 1;
    seq_t term = 1;
    for (uint64_t i = 1; i <= e; ++i) {
        term *= (seq_t)p;
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return 1;
    uint64_t x = (uint64_t)n;
    seq_t res = 1;

    /* handle powers of 2 */
    uint64_t e2 = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++e2;
    }
    if (e2 > 0) {
        seq_t pow2 = 1;
        for (uint64_t i = 0; i < e2 + 1; ++i) pow2 *= 2;
        res *= (pow2 - 1) * (pow2 - 3);
    }

    /* handle odd prime factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            seq_t sig = sigma_prime_power(p, e);
            res *= sig * sig;
        }
    }

    /* remaining prime factor, if any */
    if (x > 1) {
        seq_t sig = sigma_prime_power(x, 1);
        res *= sig * sig;
    }

    return res;
}
