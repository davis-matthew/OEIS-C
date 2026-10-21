/*
 * A052299  Minimal number of distinct prime factors of any composite number
 *          between n-th and (n+1)-st primes.
 * https://oeis.org/A052299
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of distinct prime factors of any *
 *            composite integer between the n-th prime (exclusive) and the
 *            (n+1)-st prime * (exclusive). The sequence is indexed with
 *            offset 2 (a(2) is the first * term given in the OEIS entry)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

static uint32_t *spf = NULL;   /* smallest prime factor array */
static uint32_t *primes = NULL;/* prime list */
static size_t prime_cnt = 0;   /* number of primes found */
static bool initialized = false;

/* Linear sieve (O(limit)) to fill spf[] and primes[].  limit is chosen
 * large enough to handle n up to about 10 000 (prime(10001) ≈ 104 729) with
 * ample margin. */
static void init_sieve(uint32_t limit)
{
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    primes = (uint32_t*)calloc(limit / 10 + 100, sizeof(uint32_t));  /* rough capacity */
    size_t cap = limit / 10 + 100;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt >= cap) {               /* extend primes array if needed */
                cap = cap * 2;
                primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t x = (uint64_t)i * p;
            if (x > limit) break;
            spf[(size_t)x] = p;
            if (i % p == 0) break;
        }
    }
}

/* Count distinct prime factors of n using the precomputed spf[]. */
static inline uint32_t distinct_prime_factors(uint32_t n)
{
    if (n <= 1) return 0;
    uint32_t cnt = 0;
    uint32_t last = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        if (p != last) { cnt++; last = p; }
        while (n % p == 0) n /= p;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* For indices below the offset we return 0 (undefined behavior is
     * otherwise acceptable). */
    if (n < 2) return (seq_t)0;

    /* Initialise the sieve once – 2 000 000 is ample for n up to ~10 000. */
    if (!initialized) {
        init_sieve(2000000);
        initialized = true;
    }

    /* Ensure we have primes up to n+1.  For n <= 10 000 the pre‑sieved
     * limit already contains enough primes. */
    if (prime_cnt <= (size_t)n) {
        /* Should not happen with the chosen limit, but guard against it. */
        size_t new_limit = 2 * (uint32_t)prime_cnt * 20;
        init_sieve((uint32_t)new_limit);
    }

    uint32_t p = primes[(size_t)n - 1];   /* nth prime */
    uint32_t q = primes[(size_t)n];       /* (n+1)st prime */

    uint32_t best = UINT32_MAX;

    for (uint32_t m = p + 1; m < q; ++m) {
        if (spf[m] == m) continue;                /* skip primes */
        uint32_t cnt = distinct_prime_factors(m);
        if (cnt < best) {
            best = cnt;
            if (best == 1) break;                 /* cannot be lower */
        }
    }

    return (seq_t)best;
}
