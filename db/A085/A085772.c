/*
 * A085772  (n / sum of digits of n) is a semiprime.
 * https://oeis.org/A085772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_semiprime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085772: a(n) = the n-th positive integer such that
 * (n / sum_of_digits(n)) is a semiprime.
 * The sequence starts with a(1)=10, a(2)=12, a(3)=20, ...
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_semiprime_u64.h"

/* helper: digit sum of a 64‑bit integer */
static uint64_t digit_sum_u64(uint64_t x)
{
    uint64_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* helper: sieve of Eratosthenes up to LIMIT
 * returns a vector of primes (uint32_t) */
static void sieve_primes(uint32_t limit, uint32_t **out, size_t *cnt)
{
    bool *is_composite = (bool*)malloc((limit + 1) * sizeof(bool));
    *cnt = 0;
    *out = NULL;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            (*cnt)++;
        }
        for (uint32_t j = 2; i * j <= limit; ++j) {
            is_composite[i * j] = true;
        }
    }
    *out = (uint32_t*)malloc((*cnt) * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            (*out)[idx++] = i;
        }
    }
    free(is_composite);
}

/* static caches */
static uint32_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static bool sieve_done = false;
static uint64_t last_checked = 0;
static /*restrict*/ uint64_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure primes up to at least sqrt(max_n) are available */
static void ensure_primes(uint64_t max_n)
{
    if (sieve_done) return;
    /* Estimate a reasonable upper bound for sqrt(max possible q).
     * For safety, use 4,000,000 which covers q up to ~1.6e13. */
    sieve_primes(4000000, &prime_list, &prime_list_cnt);
    sieve_done = true;
}

/* dynamic array append */
static void cache_append(uint64_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 256 : cache_cap * 2;
        uint64_t *tmp = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, but not used */

    /* ensure primes are ready for the expected range */
    ensure_primes(0);                /* primes generated once */

    /* fill cache up to required n */
    while (cache_len < (size_t)n) {
        last_checked++;
        uint64_t sum = digit_sum_u64(last_checked);
        if (sum == 0) continue;       /* not possible, safety */
        if (last_checked % sum != 0) continue;
        uint64_t q = last_checked / sum;
        if (is_semiprime_u64(q, prime_list, prime_list_cnt)) {
            cache_append(last_checked);
        }
    }

    uint64_t result = cache[n - 1];
    return (seq_t)result;
}
