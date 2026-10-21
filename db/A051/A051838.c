/*
 * A051838  Numbers k such that sum of first k primes divides product of first
 *          k primes.
 * https://oeis.org/A051838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers k such that sum of first k primes divides product
 *            of first k primes Computed by searching successive k and testing
 *            the divisibility condition
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 7299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Cache of computed sequence values (a(1), a(2), …). */
    static size_t cache_cap   = 0;
    static seq_t *cache = NULL;
    static size_t cache_len   = 0;      /* number of values stored */

    /* State for generating the sequence. */
    static size_t k_processed = 0;      /* highest k processed so far */
    static seq_t sum = 0;               /* sum of first k primes */

    /* Primes storage. */
    static uint32_t *primes = NULL;
    static size_t primes_cap   = 0;
    static size_t primes_size  = 0;
    static bool primes_ready = false;

    /* ------------------------------------------------------------------- */
    /* Generate a list of primes once.                                   */
    /* ------------------------------------------------------------------- */
    if (!primes_ready) {
        const size_t SIEVE_LIMIT = 5000000;   /* enough for n up to ~20000 */
        uint8_t *is_comp = (uint8_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
        if (!is_comp) return 0;              /* allocation failure */

        primes_cap = 500000;                 /* sufficient capacity for ~350k primes */
        primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) { free(is_comp); return 0; }

        /* Sieve of Eratosthenes */
        for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
            if (!is_comp[i]) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
        for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
            if (!is_comp[i]) {
                if (primes_size == primes_cap) {
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(is_comp); free(primes); return 0; }
                    primes = tmp;
                }
                primes[primes_size++] = (uint32_t)i;
            }
        }
        free(is_comp);
        primes_ready = true;
    }

    /* ------------------------------------------------------------------- */
    /* Extend the cache until it contains the requested term a(n).        */
    /* ------------------------------------------------------------------- */
    size_t n_needed = (size_t)n;                      /* n ≥ 1 */

    while (cache_len < n_needed) {
        /* Process next k (k = k_processed + 1). */
        ++k_processed;
        uint32_t p_k = primes[k_processed - 1];       /* k‑th prime */
        sum += (seq_t)p_k;                           /* update the sum */

        /* Test whether S_k divides the product of first k primes.      */
        bool ok = true;
        uint64_t sum64 = (uint64_t)sum;               /* fits for the required n */
        uint64_t rem   = sum64;
        uint32_t pk    = p_k;

        for (size_t pi = 0; pi < primes_size; ++pi) {
            uint32_t q   = primes[pi];
            uint64_t q64 = (uint64_t)q;
            if (q64 * q64 > rem) break;
            if (rem % q64 == 0) {
                int exp = 0;
                while (rem % q64 == 0) {
                    rem /= q64;
                    ++exp;
                    if (exp > 1) { ok = false; break; }
                }
                if (!ok) break;
                if (q > pk) { ok = false; break; }
            }
        }
        if (ok && rem > 1) {                          /* remaining prime factor */
            if (rem > pk) ok = false;
        }

        if (ok) {                                     /* append k to sequence */
            if (cache_len == cache_cap) {
                cache_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; } /* allocation failure */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k_processed;
        }
    }

    return cache[n - 1];
}
