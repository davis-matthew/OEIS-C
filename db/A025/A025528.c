/*
 * A025528  Number of prime powers <= n with exponents > 0 (A246655).
 * https://oeis.org/A025528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p prime ≤ n} ⌊log(n)/log(p)⌋ * The function counts the
 *            number of prime powers x ≤ n with exponent >0. * It uses an
 *            Eratosthenes sieve to generate all primes ≤ n and then * for
 *            each prime counts how many powers of that prime are ≤ n
 * Range:     u8 n <= 1437, u16 n <= 9684, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* no prime powers ≤ 1 */

    /* Allocate the sieve array. */
    size_t sz = (size_t)n + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) return 0;

    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= n; m += p)
            is_prime[m] = 0;
    }

    /* Count how many primes we found. */
    size_t primes_cnt = 0;
    for (idx_t p = 2; p <= n; ++p)
        if (is_prime[p]) ++primes_cnt;

    /* Store the primes in an array. */
    uint64_t *primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return 0;
    }

    size_t idx = 0;
    for (idx_t p = 2; p <= n; ++p)
        if (is_prime[p]) primes[idx++] = (uint64_t)p;

    free(is_prime);

    /* Count prime powers for each prime. */
    seq_t total = 0;
    seq_t sn = (seq_t)n;          /* convert n once to seq_t */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t cur = (seq_t)p;
        while (cur <= sn) {
            ++total;
            if (cur > sn / p) break;          /* next multiplication would overflow */
            cur *= p;
        }
    }

    free(primes);
    return total;
}
