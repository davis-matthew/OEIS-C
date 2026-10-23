/*
 * A139148  Smallest positive integer of the form (m!+n)/n.
 * https://oeis.org/A139148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + (A002034(n))!)/n (smallest positive integer of the
 *            form (m!+n)/n)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factorial.h"

/* A subset of the first few primes, sufficient for n ≤ 10 000 (√10 000 < 101). */
static const uint64_t primes[] = {
    2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t primes_cnt = sizeof(primes) / sizeof(primes[0]);

/* Return the minimal m such that n divides m!; i.e. the Smarandache function S(n). */
static idx_t S(uint64_t n)
{
    if (n <= 1) return 1;            /* S(0) and S(1) are defined as 1 */

    uint64_t temp = n;
    idx_t max_m = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;     /* remaining factor, if any, is prime */

        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }

            /* Find minimal m with Σ floor(m/p^k) ≥ e */
            idx_t m = 0;
            while (1) {
                ++m;
                uint64_t sum = 0;
                uint64_t pp = p;
                while (pp <= (uint64_t)m) {
                    sum += (uint64_t)m / pp;
                    if (pp > ((uint64_t)m / p)) break;  /* avoid overflow */
                    pp *= p;
                }
                if (sum >= e) break;
            }

            if (m > max_m) max_m = m;
        }
    }

    if (temp > 1) {                     /* residual prime factor > sqrt(n) */
        idx_t m = 0;
        /* For exponent 1 the minimal m is the prime itself. */
        m = temp;
        if (m > max_m) max_m = m;
    }

    return max_m;
}

/* Sequence A139148 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n<1; return 0 for safety */

    idx_t m = S(n);
    seq_t fact = factorial(m);          /* m! (arbitrary precision if needed) */
    seq_t result = fact / (seq_t)n;     /* n divides m! by construction */
    result += 1;                        /* (n + m!)/n = m!/n + 1 */

    return result;
}
