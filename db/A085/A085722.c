/*
 * A085722  Numbers k such that k^2 + 1 is a semiprime.
 * https://oeis.org/A085722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that k^2 + 1 is a semiprime. This implementation
 *            generates the sequence by trial‑dividing k^2+1 with a
 *            pre‑computed list of primes. It caches the results, so any call
 *            with an arbitrary n returns in O(1) after the first few calls
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve -------------------------------------------------- */
#define PRIME_LIMIT 2000000      /* > maximum k expected for n≈10 000 */

static uint32_t *prime_list = NULL;   /* array of primes */
static size_t prime_cnt = 0;          /* number of primes */

/* build primes up to PRIME_LIMIT */
static void init_primes(void)
{
    if (prime_list != NULL) return;   /* already initialized */

    size_t limit = PRIME_LIMIT;
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

    for (size_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;

    /* count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_cnt;

    /* store them */
    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (uint32_t)i;

    free(is_prime);
}

/* check whether n is a semiprime (product of exactly two primes,
   counting multiplicity) */
static bool semiprime_uint64(uint64_t n)
{
    size_t i = 0;
    int cnt = 0;
    uint64_t temp = n;

    while (i < prime_cnt)
    {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > temp) break;

        while (temp % p == 0)
        {
            temp /= p;
            ++cnt;
            if (cnt > 2) return false;   /* early exit */
        }
        ++i;
    }

    if (temp > 1) ++cnt;  /* remaining prime factor */

    return cnt == 2;
}

/* ---------- Sequence generator ------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* static state for generation */
    static idx_t next_k = 3;   /* start checking from k = 3 */

    /* ensure primes are ready */
    if (prime_list == NULL) init_primes();

    /* grow cache if needed */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* compute k^2 + 1 */
        uint64_t k = (uint64_t)next_k;
        uint64_t val = k * k + 1;

        if (semiprime_uint64(val))
        {
            /* store k in cache */
            if (cache_len >= cache_cap)
            {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
