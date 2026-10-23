/*
 * A126636  Smallest odd prime q such that pq+2 is has exactly 3 prime factors
 *          (with multiplicity), where p is the n-th odd prime.
 * https://oeis.org/A126636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Min {q odd prime | the number of prime factors (counted
 *            with multiplicity) of (p_n * q + 2) equals 3}, where p_n is the
 *            n-th odd prime (p_1 = 3)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *g_primes = NULL;   /* array of primes (starting with 2)       */
static size_t  g_prime_cnt = 0;     /* number of primes in g_primes            */
static idx_t   g_prime_limit = 0;   /* highest prime value included in g_primes */

/* Ensure that g_primes contains at least idx+1 primes (valid for idx ≤ g_prime_cnt-1). */
static void ensure_primes_upto(idx_t idx)
{
    if (g_prime_cnt > (size_t)idx)
        return;                         /* already sufficient                 */
    /* Increase limit gradually until we have enough primes. */
    idx_t new_limit = g_prime_limit ? 2 * g_prime_limit : 100;
    while (g_prime_cnt <= (size_t)idx) {
        if (new_limit < 100) new_limit = 100;
        uint64_t *tmp     = NULL;
        size_t   tmp_cnt  = 0;
        build_primes_upto(new_limit, &tmp, &tmp_cnt);
        /* Replace old array; the new array already contains all primes up
           to new_limit, so we may discard the old one. */
        free(g_primes);
        g_primes     = tmp;
        g_prime_cnt  = tmp_cnt;
        g_prime_limit = new_limit;
        new_limit *= 2;
    }
}

/* Count prime factors of n (with multiplicity) using the global prime array. */
static int count_factors(uint64_t n)
{
    int cnt = 0;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t p = g_primes[i];
        if (p * p > n) break;
        while (n % p == 0) {
            ++cnt;
            n /= p;
        }
        if (cnt > 3)          /* early exit if already more than 3          */
            return cnt;
    }
    if (n > 1) ++cnt;         /* remaining prime factor                     */
    return cnt;
}

seq_t a(idx_t n)
{
    /* Ensure we have primes up to the (n+1)-th prime (p_n). */
    ensure_primes_upto(n);
    uint64_t p = g_primes[(size_t)n];   /* p_n : n-th odd prime (p_1 = 3) */

    /* Search for the smallest odd prime q satisfying the condition. */
    for (size_t j = 1; ; ++j) {
        if (g_prime_cnt <= j)             /* need more primes for q search */
            ensure_primes_upto(j);

        uint64_t q = g_primes[j];
        if ((q & 1U) == 0) continue;      /* skip the even prime 2 */

        uint64_t prod = p * q + 2ULL;
        if (count_factors(prod) == 3)
            return (seq_t)q;               /* found the desired q         */
    }
}
