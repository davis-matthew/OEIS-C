/*
 * A076472  Number of pairs (p,q) of successive primes with p+q<=n and
 *          gcd(p+q,n)>1.
 * https://oeis.org/A076472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of pairs (p,q) of successive primes with p+q ≤ n
 *            and gcd(p+q, n) > 1. The function computes this directly by
 *            sieving primes up to n and examining adjacent pairs. The
 *            implementation uses the supplied gcd helper: #include
 *            "../utils/gcd.h"
 * Range:     u8 n <= 554, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n < 2 there are no primes, thus no qualifying pairs. */
    if (n < 2) return (seq_t)0;

    /* 1. Sieve of Eratosthenes up to n. */
    idx_t limit = n;
    uint8_t *is_prime = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (!is_prime) return (seq_t)0;     /* allocation failure, safe fallback */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    for (idx_t p = 2; p <= limit / p; ++p) {
        if (is_prime[p]) {
            for (idx_t k = p * p; k <= limit; k += p) is_prime[k] = 0;
        }
    }

    /* 2. Collect primes into a dynamic array. */
    size_t cap = 128;
    size_t cnt = 0;
    uint64_t *primes = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (cnt == cap) {
                cap <<= 1;
                uint64_t *tmp = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) {
                    free(primes);
                    free(is_prime);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[cnt++] = (uint64_t)i;
        }
    }

    /* 3. Count qualifying consecutive prime pairs. */
    seq_t count = 0;
    for (size_t idx = 0; idx + 1 < cnt; ++idx) {
        uint64_t p = primes[idx];
        uint64_t q = primes[idx + 1];
        uint64_t sum = p + q;
        if (sum > n) break;            /* sums will only increase thereafter */
        seq_t g = gcd((seq_t)sum, (seq_t)n);
        if (g > (seq_t)1) ++count;
    }

    free(primes);
    free(is_prime);
    return count;
}
