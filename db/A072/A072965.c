/*
 * A072965  In prime factorization of n replace all matching twin prime pairs
 *          with 1, where (3,5)-matches are replaced before (5,7).
 * https://oeis.org/A072965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A072965: Prime factor replacement with twin primes
   a(n) = n with each twin prime pair (p,p+2) replaced by 1;
   pairs are processed in increasing order of the lower prime.
   The result is the product of the remaining prime powers. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */
    uint64_t m = (uint64_t)n;
    uint64_t temp = m;

    /* small arrays to hold prime factorization */
    const int MAX_FACT = 64;
    uint64_t primes[MAX_FACT];
    uint64_t exps[MAX_FACT];
    int pfCount = 0;

    /* trial division prime factorization */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pfCount] = p;
            exps[pfCount]   = e;
            ++pfCount;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        primes[pfCount] = temp;
        exps[pfCount]   = 1;
        ++pfCount;
    }

    /* cancel twin prime pairs in order of lower prime */
    for (int i = 0; i < pfCount; ++i) {
        for (int j = i + 1; j < pfCount; ++j) {
            if (primes[j] == primes[i] + 2) {
                uint64_t k = exps[i] < exps[j] ? exps[i] : exps[j];
                if (k) {
                    exps[i] -= k;
                    exps[j] -= k;
                }
            }
        }
    }

    /* compute product of remaining prime powers */
    seq_t result = 1;
    for (int i = 0; i < pfCount; ++i) {
        for (uint64_t e = 0; e < exps[i]; ++e) {
            result *= (seq_t)primes[i];
        }
    }
    return result;
}
