/*
 * A036330  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 6.
 * https://oeis.org/A036330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, easy
 * Formula:   none (definition based)
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: composite numbers whose concatenated prime factors
 * (listed in increasing order) have exactly six decimal digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_small.h"  /* is_prime_small(seq_t n) */

/* --------------------------------------------------------------------- */
/*  Helper: simple sieve of Eratosthenes to generate primes up to a limit.
 *  The limit is increased until it is large enough for the required terms. */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    if (primes != NULL)
        return;
    const uint32_t LIMIT = 200000;   /* safe upper bound for n <= 10000 */
    bool *comp = (bool*)malloc(LIMIT + 1);
    memset(comp, 0, LIMIT + 1);
    primes = (uint32_t*)malloc(LIMIT / 10 * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!comp[i]) {
            primes[idx++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    comp[j] = true;
            }
        }
    }
    primes_cnt = idx;
    free(comp);
}

/* --------------------------------------------------------------------- */
/* Count decimal digits of a 64‑bit unsigned integer. */
static inline int digits_uint64(uint64_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* For a composite integer n (uint64_t) compute the total digit length
 * of its prime factors, each factor repeated according to its exponent. */
static size_t factor_digit_sum(uint64_t n)
{
    size_t sum = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        if (exp > 0)
            sum += digits_uint64(p) * exp;
    }
    if (n > 1) {            /* remaining prime factor */
        sum += digits_uint64(n);
    }
    return sum;
}

/* --------------------------------------------------------------------- */
/* Cache of sequence values and state. */
static seq_t  *cache = NULL;      /* array of found terms */
static size_t  cache_size = 0;    /* number of terms stored   */
static size_t  cache_cap  = 0;    /* allocated capacity       */
static seq_t   next_candidate = 4;/* next integer to test     */

/* Grow the cache array if necessary. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Ensure that cache holds at least n terms.  When more terms are needed
 * we iterate over integers, testing the definition until we reach count n. */
static void ensure_cache_size(idx_t n)
{
    init_primes();          /* make sure prime list is ready */
    while (cache_size < n) {
        seq_t val = next_candidate++;
        if (val < 4)
            continue;                          /* ignore 1,2,3 */
        if (is_prime_small(val))
            continue;                          /* exclude primes */

        size_t dsum = factor_digit_sum((uint64_t)val);
        if (dsum == 6) {
            if (cache_size == cache_cap)
                grow_cache();
            cache[cache_size++] = val;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Public function required by the build system.                         */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1; callers should not ask for 0   */
        return 0;
    ensure_cache_size(n);
    return cache[n-1];
}
