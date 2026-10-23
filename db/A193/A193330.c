/*
 * A193330  Number of prime factors of n^2 + 1, counted with multiplicity.
 * https://oeis.org/A193330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of n^2 + 1 (with multiplicity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Precomputed primes up to 10000 – enough for n ≤ 10 000 */
    static uint32_t primes[1229];
    static size_t prime_cnt = 0;

    if (prime_cnt == 0) {
        /* Simple sieve of Eratosthenes up to 10000 */
        static uint8_t is_prime[10001];
        for (size_t i = 0; i <= 10000; ++i) {
            is_prime[i] = 1;
        }
        is_prime[0] = is_prime[1] = 0;
        for (uint32_t i = 2; (uint64_t)i * i <= 10000; ++i) {
            if (is_prime[i]) {
                for (uint32_t j = i * i; j <= 10000; j += i) {
                    is_prime[j] = 0;
                }
            }
        }
        size_t idx = 0;
        for (uint32_t i = 2; i <= 10000; ++i) {
            if (is_prime[i]) {
                primes[idx++] = i;
            }
        }
        prime_cnt = idx;
    }

    /* Compute m = n^2 + 1; use 64‑bit arithmetic to avoid overflow. */
    uint64_t n64 = (uint64_t)n;
    uint64_t m = n64 * n64 + 1ULL;

    seq_t count = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) {
            break;
        }
        while (m % p == 0) {
            ++count;
            m /= p;
        }
    }

    /* Any remaining factor > 1 is prime. */
    if (m > 1) {
        ++count;
    }

    return count;
}
