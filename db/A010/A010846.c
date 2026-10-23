/*
 * A010846  Number of numbers <= n whose set of prime factors is a subset of
 *          the set of prime factors of n.
 * https://oeis.org/A010846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |{ k ≤ n : the set of prime factors of k is a subset of
 *            the set of prime factors of n }| i.e. every prime dividing k
 *            also divides n
 * Range:     u8 n <= 2309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* a(n) – OEIS A010846; external linkage required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but handle n=0 safely */
    seq_t ans = 0;                 /* result counter */

    for (idx_t k = 1; k <= n; ++k) {
        uint64_t temp = (uint64_t)k;
        bool ok = true;

        /* trial division to factor k, checking divisors against n */
        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                /* d is a prime factor of k */
                if ((n % d) != 0) { /* d does not divide n → fail */
                    ok = false;
                    break;
                }
                /* remove all powers of d from temp */
                while (temp % d == 0) temp /= d;
            }
        }
        if (!ok) continue; /* k not counted */

        /* if remainder is a prime factor > sqrt(k) */
        if (temp > 1 && (n % temp) != 0) {
            continue;
        }

        /* successful: all prime factors of k divide n */
        ++ans;
    }

    return ans;
}
