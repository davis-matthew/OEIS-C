/*
 * A174273  Inverse Moebius transform of A035263.
 * https://oeis.org/A174273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(1) = 1 Multiplicative: a(p^e) = e + 1 for odd prime p a(2^e) =
 *            e + 1 – floor((e + 1)/2) This follows the OEIS description of
 *            A174273
 * Range:     u8 n <= 9942, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static void _sieve_primes(uint64_t limit, uint64_t *pr, size_t *cnt)
{
    if (limit < 2) { *cnt = 0; return; }
    bool *is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
    size_t k = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[k++] = i;
    free(is_prime);
    *cnt = k;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Build a small prime table once (primes up to 10000). */
    static bool init = false;
    static uint64_t primes[1200];
    static size_t prime_cnt = 0;
    if (!init) {
        _sieve_primes(10000, primes, &prime_cnt);
        init = true;
    }

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p != 0) continue;
        unsigned exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        seq_t val = (seq_t)(exp + 1);
        if (p == 2)
            val -= val / (seq_t)2;   /* subtract floor((e+1)/2) */
        result *= val;
    }
    if (temp > 1) {            /* remaining prime factor (> sqrt(n)) */
        seq_t val = (seq_t)2;   /* exponent = 1, so e+1 = 2 */
        if (temp == 2)
            val -= val / (seq_t)2;   /* apply 2^e rule */
        result *= val;
    }
    return result;
}
