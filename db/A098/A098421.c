/*
 * A098421  Primes occurring in exactly one prime triple (p,q,r) with
 *          p<q<r=p+6.
 * https://oeis.org/A098421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime that occurs in exactly one prime triple
 *            (p,q,r) with p < q < r = p + 6. The function generates all
 *            primes up to a fixed limit, counts how many triple occurrences
 *            each prime appears in, and stores the primes that occur exactly
 *            once. Subsequent calls reuse the cached tables
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static variables for caching */
    static bool      initialized = false;
    static uint8_t  *is_prime     = NULL;            /* 1 = prime, 0 = composite */
    static uint16_t *cnt          = NULL;            /* occurrence counts */
    static uint32_t *primes       = NULL;            /* list of primes */
    static seq_t   *res_list     = NULL;            /* primes with cnt==1 */
    static idx_t    res_cnt      = 0;                /* number of such primes */

    /* Define sieve limit.  5,000,000 is large enough for n up to ~10,000. */
    static const uint32_t LIMIT = 5000000;

    /* One‑time initialization */
    if (!initialized) {
        /* Allocate and initialise the is_prime array */
        is_prime = (uint8_t *)malloc((LIMIT + 1) * sizeof(uint8_t));
        if (!is_prime) return (seq_t)0;
        memset(is_prime, 1, LIMIT + 1);
        is_prime[0] = is_prime[1] = 0;

        /* Sieve of Eratosthenes */
        for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) {
                    is_prime[j] = 0;
                }
            }
        }

        /* Count primes to allocate initial array */
        uint32_t initial_cap = LIMIT / 10 + 100;  /* safe upper bound */
        primes = (uint32_t *)malloc(initial_cap * sizeof(uint32_t));
        if (!primes) return (seq_t)0;
        uint32_t prime_cnt = 0;

        /* Collect primes into the array */
        for (uint32_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[i]) {
                if (prime_cnt >= initial_cap) {
                    /* double the capacity if necessary */
                    initial_cap *= 2;
                    uint32_t *tmp = (uint32_t *)realloc(primes,
                                         initial_cap * sizeof(uint32_t));
                    if (!tmp) return (seq_t)0;
                    primes = tmp;
                }
                primes[prime_cnt++] = i;
            }
        }

        /* Allocate count array (indexed by prime value) */
        cnt = (uint16_t *)calloc(LIMIT + 1, sizeof(uint16_t));
        if (!cnt) return (seq_t)0;

        /* Count occurrences in triples */
        for (uint32_t idx = 0; idx < prime_cnt; ++idx) {
            uint32_t p = primes[idx];
            uint32_t r = p + 6;
            if (r > LIMIT || !is_prime[r]) continue;

            uint32_t q_start = idx + 1;
            uint32_t q_count = 0;
            while (q_start < prime_cnt && primes[q_start] < r) {
                cnt[primes[q_start]]++;  /* q occurs once per triple */
                ++q_start;
                ++q_count;
            }
            if (q_count > 0) {
                cnt[p] += q_count;       /* p occurs once per each q */
                cnt[r] += q_count;       /* r occurs once per each q */
            }
        }

        /* Build the result list of primes with count == 1 */
        res_list = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
        if (!res_list) return (seq_t)0;
        res_cnt = 0;
        for (uint32_t i = 0; i < prime_cnt; ++i) {
            uint32_t v = primes[i];
            if (cnt[v] == 1) {
                res_list[res_cnt++] = (seq_t)v;
            }
        }

        initialized = true;
    }

    /* Return the n-th term (1‑based).  Return 0 for out‑of‑range indices. */
    if (n == 0 || n > res_cnt) return (seq_t)0;
    return res_list[n - 1];
}
