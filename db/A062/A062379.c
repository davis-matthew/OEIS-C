/*
 * A062379  n divided by largest 4th-power-free factor of n.
 * https://oeis.org/A062379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n / (largest 4‑th‑power‑free factor of n) Equivalent:
 *            a(p^e) = p^max(e-3, 0) for each prime power in n
 * Range:     u8 n <= 2047, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 0 return 1 (out of range, harmless) */
    if (n < 1) return 1;

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately for speed */
    uint64_t p = 2;
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp;
    }
    if (exp >= 4) {
        seq_t pow = 1;
        for (int i = 0; i < exp - 3; ++i)
            pow *= p;          /* multiply by 2 */
        result *= pow;
    }

    /* Factor odd primes by trial division */
    for (p = 3; p * p <= m; p += 2) {
        exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp >= 4) {
            seq_t pow = 1;
            for (int i = 0; i < exp - 3; ++i)
                pow *= (seq_t)p;   /* multiply by current prime */
            result *= pow;
        }
    }

    /* If a prime factor remains (>1), its exponent is 1 (<4),
       so it contributes nothing to the product. */

    return result;
}
