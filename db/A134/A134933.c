/*
 * A134933  a(n)=A134930(n)/2.
 * https://oeis.org/A134933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate A134933 by enumerating twin primes (p,p+2),
   producing the three integers (p-1)/2, (p+1)/2, (p+3)/2.
   Values are stored only if strictly greater than the last stored value,
   ensuring the resulting sequence is sorted and free of duplicates. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Adjust sieve limit if more terms are required. */
static const idx_t SIEVE_LIMIT = 2000000ULL;

/* Prime sieve data. */
static char *is_prime      = NULL;
static uint32_t *primes   = NULL;
static size_t primes_count = 0;
static size_t primes_cap   = 0;
static bool sieve_done = false;

/* Cached sequence values. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Index of the next prime to test for a twin pair. */
static size_t twin_index = 0;

/* Append a new value to the cache, expanding storage if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* Initialise the prime sieve and prime list. */
static void init_sieve(void)
{
    if (sieve_done) return;
    sieve_done = true;

    is_prime = (char*)malloc((size_t)(SIEVE_LIMIT + 1));
    if (!is_prime) abort();

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    primes_cap = 150000;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) abort();

    primes_count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) {
            if (primes_count >= primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_count++] = (uint32_t)i;
        }
}

/* Generate sequence values until at least `target` terms are stored. */
static void generate_until(size_t target)
{
    init_sieve();

    while (cache_len < target) {
        /* Scan for the next twin prime pair. */
        while (twin_index + 1 < primes_count) {
            uint32_t p = primes[twin_index];
            if (p + 2U == primes[twin_index + 1]) {
                /* Process the three candidate values. */
                seq_t v1 = ((seq_t)p - 1U) / 2U;
                seq_t v2 = ((seq_t)p + 1U) / 2U;
                seq_t v3 = ((seq_t)p + 3U) / 2U;

                if (cache_len == 0 || v1 > cache[cache_len - 1]) cache_append(v1);
                if (cache_len == 0 || v2 > cache[cache_len - 1]) cache_append(v2);
                if (cache_len == 0 || v3 > cache[cache_len - 1]) cache_append(v3);

                ++twin_index; /* move past the first prime of this pair */
                break;
            }
            ++twin_index;
        }

        /* Out of primes: should not happen for n <= 10000 with SIEVE_LIMIT=2M */
        if (twin_index + 1 >= primes_count) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based; offset is 1 */

    generate_until((size_t)n);
    return cache[(size_t)(n - 1)];
}
