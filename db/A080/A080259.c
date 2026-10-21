/*
 * A080259  Numbers whose squarefree kernel is not a primorial number, i.e.,
 *          A007947(a(n)) is not in A002110.
 * https://oeis.org/A080259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers whose squarefree kernel is not a primorial
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Compute the n-th term of A080259 (offset 1). */
seq_t a(idx_t n)
{
    /* Static initialization data */
    static bool initialized = false;
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_candidate = 1;   /* candidate to test next */

    /* ------------------------------------------------------------------
       Initialize primes once.  A generous limit guarantees enough primes
       for n up to about 10 000, keeping candidate values well below 2 000 000.
       ------------------------------------------------------------------ */
    if (!initialized) {
        const size_t LIMIT = 200000;  /* primes up to 200 000 are sufficient here */
        bool *is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
        if (!is_prime) return (seq_t)0;  /* allocation failed */

        /* trivial primes */
        for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

        /* sieve of Eratosthenes */
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
            }
        }

        /* count primes first to allocate exact size */
        size_t cnt = 0;
        for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

        primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!primes) {
            free(is_prime);
            return (seq_t)0;
        }

        size_t idx = 0;
        for (size_t i = 2; i <= LIMIT; ++i) {
            if (is_prime[i]) primes[idx++] = (uint64_t)i;
        }

        free(is_prime);
        primes_cnt = cnt;
        initialized = true;
    }

    /* ------------------------------------------------------------------
       Compute more terms if necessary
       ------------------------------------------------------------------ */
    while (cache_len < (size_t)n) {
        /* Find next candidate that satisfies the condition */
        uint64_t k = last_candidate++;
        /* Compute squarefree kernel */
        uint64_t temp = k;
        uint64_t kernel = 1;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
            if (temp % primes[i] == 0) {
                kernel *= primes[i];
                while (temp % primes[i] == 0) temp /= primes[i];
            }
        }
        if (temp > 1) kernel *= temp;   /* remaining prime factor > sqrt(k) */

        /* Check whether kernel is a primorial (product of first m primes, m ≥ 0) */
        bool is_primorial = false;
        if (kernel == 1) {
            is_primorial = true;          /* 1 is 0# */
        } else {
            uint64_t t = kernel;
            for (size_t i = 0; i < primes_cnt; ++i) {
                if (t % primes[i] != 0) break;   /* missing a required prime */
                t /= primes[i];
                if (t == 1) {   /* all required primes consumed */
                    is_primorial = true;
                    break;
                }
            }
        }

        if (!is_primorial) {
            /* Store the term in cache, expanding if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *newbuf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!newbuf) return (seq_t)0;  /* allocation failed */
                cache = newbuf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
