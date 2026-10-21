/*
 * A093320  a(1) = 1; for m >= 2, a(m) = sum{p|m} a(pi(p)), where the sum is
 *          over the distinct prime divisors p of m and pi(p) is the order of
 *          p among the primes = the number of primes <= p.
 * https://oeis.org/A093320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A093320
 *  a(1) = 1
 *  for m ≥ 2,  a(m) = Σ_{p‖m} a(π(p)),
 *  where the sum is over distinct prime divisors p of m
 *  and π(p) is the number of primes ≤ p.
 *
 *  The routine keeps a memoised array of a(1)…a(n) and
 *  precomputes a sieve of smallest prime factors and π
 *  up to the maximum n requested.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t max_cache = 0;            /* highest n cached */
    static seq_t *cache   = NULL;          /* a(1)..a(max_cache) */
    static uint64_t *spf  = NULL;          /* smallest prime factor */
    static idx_t *pi      = NULL;          /* prime counting function */

    /* If the requested n is already cached, return it */
    if (n <= max_cache && cache != NULL)
        return cache[n];

    /* Allocate / resize storage for the new maximum n */
    idx_t new_max = n;
    seq_t *new_cache = (seq_t*)realloc(cache, (new_max + 1) * sizeof(seq_t));
    uint64_t *new_spf = (uint64_t*)realloc(spf, (new_max + 1) * sizeof(uint64_t));
    idx_t *new_pi = (idx_t*)realloc(pi, (new_max + 1) * sizeof(idx_t));

    if (!new_cache || !new_spf || !new_pi) {
        /* Allocation failed – something terribly wrong in tests */
        return (seq_t)0;
    }

    cache = new_cache;
    spf   = new_spf;
    pi    = new_pi;
    max_cache = new_max;

    /* Initialise sieve arrays for all indices 0..n */
    cache[0] = 0;          /* unused */
    spf[0]   = 0;
    spf[1]   = 1;
    pi[0]    = 0;
    pi[1]    = 0;
    cache[1] = 1;          /* a(1) = 1 */

    /* Initialise spf and prime flag */
    for (idx_t i = 2; i <= n; ++i) {
        spf[i] = i;        /* assume prime initially */
    }

    /* Sieve of Eratosthenes with smallest prime factor assignment */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf[j] == j)           /* first encounter */
                    spf[j] = i;           /* record smallest prime factor */
            }
        }
    }

    /* Compute prime counting function π(i) while building sieve */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == i)                    /* prime check */
            ++prime_cnt;
        pi[i] = prime_cnt;
    }

    /* Compute a(m) for m = 2 … n */
    for (idx_t m = 2; m <= n; ++m) {
        seq_t sum = 0;
        idx_t m_copy = m;
        uint64_t last_p = 0;

        /* Iterate over distinct prime divisors using spf */
        while (m_copy > 1) {
            uint64_t p = spf[m_copy];
            if (p == 0) p = m_copy;              /* safety fallback */
            if (p != last_p) {
                idx_t pi_val = pi[p];
                sum += cache[pi_val];
                last_p = p;
            }
            /* divide out all powers of p */
            do
                m_copy /= p;
            while (m_copy % p == 0);
        }

        cache[m] = sum;
    }

    return cache[n];
}
