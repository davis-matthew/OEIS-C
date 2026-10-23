/*
 * A107745  Smallest prime factor of 6*n-1.
 * https://oeis.org/A107745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = smallest prime factor of 6*n − 1 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* add a prime, expanding storage if necessary */
static void add_prime(uint32_t p)
{
    if (primes_cnt >= primes_cap) {
        size_t new_cap = (primes_cap == 0) ? 16 : primes_cap << 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (tmp == NULL)
            return;
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* extend the prime list up to a given limit (inclusive) */
static void build_primes_upto(uint32_t limit)
{
    if (primes_cnt > 0 && primes[primes_cnt - 1] >= limit)
        return;

    uint32_t start = (primes_cnt == 0) ? 5 : primes[primes_cnt - 1] + 2;
    for (uint32_t p = start; p <= limit; p += 2) {
        int isprime = 1;
        uint32_t r = (uint32_t)sqrt((double)p);
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t q = primes[i];
            if (q > r)
                break;
            if (p % q == 0) {
                isprime = 0;
                break;
            }
        }
        if (isprime)
            add_prime(p);
    }
}

seq_t a(idx_t n)
{
    /* m = 6*n - 1 */
    seq_t m = (seq_t)6 * (seq_t)n - 1;

    /* approximate m for sqrt bound (uses 64‑bit arithmetic) */
    unsigned long long m_ll;
#if defined(__cplusplus)
    m_ll = static_cast<unsigned long long>(m);
#else
    m_ll = (unsigned long long)(m);
#endif

    uint32_t limit = (uint32_t)(sqrtl((long double)m_ll) + 1.0L);
    if (limit < 5)
        limit = 5;
    build_primes_upto(limit);

    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        if (p * p > m)
            break;
        if (m % p == 0)
            return p;
    }

    /* m itself is prime */
    return m;
}
