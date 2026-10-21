/*
 * A061243  a(n) = n+r where r is the smallest number such that n divides
 *          (n+1)(n+2)(n+3)...(n+r).
 * https://oeis.org/A061243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + A002034(n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   minimal_m_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002034(n) : smallest m such that n | m! (Smarandache function). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/minimal_m_prime_power.h"

/* Thread‑safe, static prime list generation used by the factoriser. */
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_alloc = 0;

/* Build (or rebuild) a list of primes up to the given limit.  The
   list is replaced if the requested limit is larger than the current
   allocation.  prime_cnt is reset to 0 when rebuilding. */
static void ensure_primes(size_t limit)
{
    if (prime_alloc >= limit)
        return;

    free(primes);
    prime_alloc = limit + 1;
    primes = (uint64_t *)malloc(prime_alloc * sizeof(uint64_t));
    bool *composite = (bool *)calloc(prime_alloc, 1);
    prime_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    composite[j] = true;
            }
        }
    }
    free(composite);
}

/* Return the Smarandache function S(n) : minimal m with n | m!.  For
   n == 1 the result is 1 by definition.  For n > 1, factorise n and
   take the maximum of the required m for each prime power factor. */
static seq_t smarandache_value(idx_t n)
{
    if (n == 1)
        return 1;     /* r = 1 → a(1) = 2 */

    uint64_t nn = (uint64_t)n;
    seq_t rmax  = 0;

    /* Build a prime table up to sqrt(nn). */
    size_t lim = (size_t)(sqrt((long double)nn) + 1);
    ensure_primes(lim);

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= nn; ++i) {
        uint64_t p = primes[i];
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            seq_t m = minimal_m_prime_power(p, e);
            if (m > rmax)
                rmax = m;
        }
    }

    if (nn > 1) { /* remaining prime factor > sqrt(original n) */
        seq_t m = minimal_m_prime_power(nn, 1);
        if (m > rmax)
            rmax = m;
    }

    return rmax;
}

/* Public sequence function used by the test harness. */
seq_t a(idx_t n)
{
    seq_t r = smarandache_value(n);
    return (seq_t)n + r;
}
