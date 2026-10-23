/*
 * A125868  Odd numbers k such that cos(2*Pi/k) is an algebraic number of a
 *          5-smooth degree, but not 3-smooth.
 * https://oeis.org/A125868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th odd integer k for which φ(k) is a 5‑smooth
 *            integer divisible by 5. (φ is Euler’s totient)
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool primes_initialized = false;
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Simple sieve generating all primes up to 1'000'000. */
static void init_primes(void)
{
    if (primes_initialized) return;
    const uint32_t limit = 1000000;
    bool *is_comp = (bool*)malloc((limit + 1) * sizeof(bool));
    memset(is_comp, 0, (limit + 1) * sizeof(bool));
    prime_count = 0;
    prime_list = (uint32_t*)malloc((limit / 10) * sizeof(uint32_t));  /* oversize */
    for (uint32_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            prime_list[prime_count++] = p;
            if ((uint64_t)p * p <= limit) {
                for (uint32_t q = p * p; q <= limit; q += p)
                    is_comp[q] = true;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* Compute φ(k) for an odd integer k. We assume k fits in uint64_t. */
static uint64_t phi_odd(uint64_t k)
{
    uint64_t result = k;
    uint64_t n = k;
    init_primes();
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            result = result / p * (p - 1);
            do { n /= p; } while (n % p == 0);
        }
    }
    if (n > 1) {
        result = result / n * (n - 1);
    }
    return result;
}

/* Check whether φ(k) is 5‑smooth (all prime factors 2,3,5) and divisible by 5. */
static bool good_k(uint64_t k)
{
    uint64_t t = phi_odd(k);
    if (t % 5 != 0) return false;           /* must be divisible by 5 */

    bool has5 = false;
    while (t % 2 == 0) t /= 2;
    while (t % 3 == 0) t /= 3;
    while (t % 5 == 0) { t /= 5; has5 = true; }

    return has5 && t == 1;
}

/* Dynamic cache of the generated sequence. */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static unsigned long long last_checked = 3;   /* next odd integer to test */

/* Append a value to the cache, expanding it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == 0) {
        seq_cache = (seq_t*)malloc(sizeof(seq_t) * 64);
    } else {
        seq_cache = (seq_t*)realloc(seq_cache, sizeof(seq_t) * (cache_len + 64));
    }
    seq_cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used, for safety */
    while (cache_len < (size_t)n) {
        /* find next odd k that satisfies the property */
        while (true) {
            uint64_t k = last_checked;
            last_checked += 2;
            if (good_k(k)) {
                cache_append((seq_t)k);
                break;
            }
        }
    }
    return seq_cache[n - 1];
}
