/*
 * A053723  Number of 5-core partitions of n.
 * https://oeis.org/A053723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult, changed
 * Formula:   a(n) = product over prime powers of (n+1) of the multiplicative
 *            function * b(p^e) where * b(5^e) = 5^e * if p ≡ 1 or 4 (mod 5):
 *            b(p^e) = (p^(e+1) – 1)/(p – 1) * if p ≡ 2 or 3 (mod 5): b(p^e) =
 *            (p^(e+1) + (–1)^e)/(p + 1) * and a(n) = b(n+1). Thus a(n) is the
 *            number of 5‑core partitions of n
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bprime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bprime.h"

/* main sequence function */
seq_t a(idx_t n)
{
    uint64_t m = n + 1;          /* we need b(m) */
    if (m == 1) return 1;        /* a(0) = 1 */

    seq_t result = 1;
    uint64_t temp = m;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint8_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= bprime(p, e);
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        result *= bprime(temp, 1);
    }

    return result;
}
