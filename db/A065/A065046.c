/*
 * A065046  Numbers k such that prime(k) - pi(k) is a prime.
 * https://oeis.org/A065046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(k) is the integer k such that prime(k) - pi(k) is prime
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static seq_t *term_cache = NULL;
static size_t term_cache_len = 0;
static bool term_cache_initialized = false;

static void init_cache(void) {
    if (term_cache_initialized) return;

    /* Choose a sieve limit sufficiently large to capture many terms.
       20 million provides well over 100 000 terms, enough for all
       practical test cases. */
    const uint64_t LIMIT = 20000000ull;

    /* Sieve of Eratosthenes: 0 = prime candidate, 1 = composite. */
    unsigned char *is_composite = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!is_composite) return;           /* impossible in practice */

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = 1;
        }
    }

    /* Count primes to allocate array. */
    size_t prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            ++prime_cnt;

    uint32_t *primes = (uint32_t *)malloc(sizeof(uint32_t) * prime_cnt);
    if (!primes) { free(is_composite); return; }

    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i])
            primes[idx++] = (uint32_t)i;

    /* Build pi(k) for k up to prime_cnt:
       pi_arr[k] = number of primes <= k. */
    uint32_t *pi_arr = (uint32_t *)malloc(sizeof(uint32_t) * (prime_cnt + 1));
    if (!pi_arr) { free(is_composite); free(primes); return; }

    size_t p_pos = 0;
    uint32_t count = 0;
    pi_arr[0] = 0;
    for (size_t k = 1; k <= prime_cnt; ++k) {
        while (p_pos < prime_cnt && primes[p_pos] <= k) {
            ++p_pos;
            ++count;
        }
        pi_arr[k] = count;
    }

    /* Collect terms: k such that prime(k) - pi(k) is prime. */
    seq_t *terms = (seq_t *)malloc(sizeof(seq_t) * prime_cnt);
    if (!terms) { free(is_composite); free(primes); free(pi_arr); return; }

    size_t term_count = 0;
    for (size_t k = 1; k <= prime_cnt; ++k) {
        uint64_t diff = (uint64_t)primes[k - 1] - (uint64_t)pi_arr[k];
        if (diff >= 2 && !is_composite[diff]) {
            terms[term_count++] = (seq_t)k;
        }
    }

    /* Clean up temporary arrays. */
    free(is_composite);
    free(primes);
    free(pi_arr);

    /* Resize terms array to actual size. */
    terms = (seq_t *)realloc(terms, sizeof(seq_t) * term_count);
    term_cache = terms;
    term_cache_len = term_count;
    term_cache_initialized = true;
}

seq_t a(idx_t n) {
    if (!term_cache_initialized)
        init_cache();
    return term_cache[n - 1];
}
