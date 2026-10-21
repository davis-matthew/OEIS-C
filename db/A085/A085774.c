/*
 * A085774  Numbers k such that (k / sum of digits of k) and (k+1 / sum of
 *          digits of k+1) are both semiprime.
 * https://oeis.org/A085774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: exhaustive search for the A085774 sequence – the n‑th k such that
   (k / sumdigits(k)) and ((k+1) / sumdigits(k+1)) are both semiprime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/sum_distinct_prime_factors.h"   /* not used but avoids missing include */
/* (The list of utilities is long; only the standard headers are needed for this implementation.) */

/* ---------------------------------------------------------------------- */
/* Prime table – generated once, up to PRIME_LIMIT                           */
#define PRIME_LIMIT 1000000U   /* 1 000 000 is enough for probable quotients up to around 1e12 */
static uint32_t *prime_table = NULL;
static size_t prime_count = 0;
static bool primes_prepared = false;

static void prepare_primes(void)
{
    if (primes_prepared) return;
    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!sieve) return;   /* allocation failure: leave primes empty */
    for (uint32_t i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint32_t p = 2; (uint64_t)p * p <= PRIME_LIMIT; ++p)
        if (sieve[p])
            for (uint32_t j = p * p; j <= PRIME_LIMIT; j += p)
                sieve[j] = false;
    prime_table = (uint32_t*)malloc(PRIME_LIMIT * sizeof(uint32_t));
    if (!prime_table) { free(sieve); return; }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_table[prime_count++] = i;
    free(sieve);
    primes_prepared = true;
}

/* ---------------------------------------------------------------------- */
/* Simple semiprime tester for 64‑bit integers.  Exactly two prime factors
   (counted with multiplicity). */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4) return false;   /* 4 = 2×2 is the smallest semiprime */
    uint64_t tmp = n;
    int cnt = 0;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_table[i] * prime_table[i] <= tmp; ++i) {
        uint32_t p = prime_table[i];
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (tmp > 1) ++cnt;      /* remaining prime factor */
    return cnt == 2;
}

/* ---------------------------------------------------------------------- */
/* Sum of decimal digits – works for any n that fits in uint64_t            */
static uint64_t digit_sum_uint64(uint64_t n)
{
    uint64_t s = 0;
    while (n) { s += n % 10; n /= 10; }
    return s;
}

/* ---------------------------------------------------------------------- */
/* Caching of sequence values                                              */
static seq_t *term_cache  = NULL;
static idx_t  cache_len   = 0;
static idx_t  cache_cap   = 0;
static uint64_t next_k = 1;      /* next integer to examine */

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    prepare_primes();   /* ensure primes are ready */

    /* Return cached value if already computed */
    if (n <= cache_len) return term_cache[n - 1];

    /* Expand cache until we have the n-th term */
    while (cache_len < n) {
        /* Find next qualifying k */
        for (;;) {
            uint64_t k = next_k;
            uint64_t s_k = digit_sum_uint64(k);
            if (s_k == 0 || k % s_k != 0) { ++next_k; continue; }
            uint64_t q_k = k / s_k;
            if (!is_semiprime_uint64(q_k)) { ++next_k; continue; }

            uint64_t kp = k + 1;
            uint64_t s_kp = digit_sum_uint64(kp);
            if (s_kp == 0 || kp % s_kp != 0) { ++next_k; continue; }
            uint64_t q_kp = kp / s_kp;
            if (!is_semiprime_uint64(q_kp)) { ++next_k; continue; }

            /* Both conditions satisfied – store term */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(term_cache, cache_cap * sizeof(seq_t));
                if (!tmp) { cache_cap = 0; free(term_cache); term_cache = NULL; return 0; }
                term_cache = tmp;
            }
            term_cache[cache_len++] = (seq_t)k;
            ++next_k;
            break;                     /* move to next missing term */
        }
    }

    return term_cache[n - 1];
}
