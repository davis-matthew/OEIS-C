/*
 * A117494  a(n) is the number of m's, 1 <= m <= n, where gcd(m,n) is prime.
 * https://oeis.org/A117494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} phi(n/p)
 * Range:     u8 n <= 513, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 8867 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t result = 0;
    idx_t temp = n;

    /* handle factor 2 separately */
    if ((temp & 1) == 0) {
        result += totient(n / 2);
        do { temp >>= 1; } while ((temp & 1) == 0);
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            result += totient(n / p);
            do { temp /= p; } while (temp % p == 0);
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (temp > 1)
        result += totient(n / temp);

    return result;
}
