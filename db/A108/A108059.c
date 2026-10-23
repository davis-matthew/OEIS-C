/*
 * A108059  Numbers k such that 181*k + 179 is prime.
 * https://oeis.org/A108059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 4363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A108059: k such that 181*k + 179 is prime.
 * Computed by a simple sieve over increasing k, stopping when n terms
 * have been found.  Values are stored in a static cache so that any
 * order of calls works.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
 *  Small prime precomputation (up to 200 000) used for trial division.
 * ------------------------------------------------------------------ */
static uint32_t *small_primes = NULL;
static size_t   small_prime_count = 0;

static void init_small_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* The limit is large enough to have all prime factors of 181*k+179
       for k up to ~2·10⁴.  It works for higher k as well because we
       only use it for trial division and the sieve will never be
       required beyond its square root. */
    const size_t LIMIT = 200000u;

    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) return;               /* allocation failure → silent abort */

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= LIMIT; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= LIMIT; j += p)
                is_prime[j] = false;
        }
    }

    size_t count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++count;

    small_primes = (uint32_t*)malloc(count * sizeof(uint32_t));
    if (!small_primes) { free(is_prime); return; }

    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) small_primes[idx++] = (uint32_t)i;

    small_prime_count = count;
    free(is_prime);
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < small_prime_count; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) return n == p;
    }
    return true;
}

/* ------------------------------------------------------------------
 *  Cache of previously computed terms.
 * ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static size_t  cache_size = 0;
static size_t  cache_capacity = 0;

static void ensure_cache_capacity(size_t new_size)
{
    if (new_size <= cache_capacity) return;
    size_t new_cap = (cache_capacity == 0) ? 16 : cache_capacity * 2;
    while (new_cap < new_size) new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                     /* allocation failure → silent abort */
    cache = tmp;
    cache_capacity = new_cap;
}

/* ------------------------------------------------------------------
 *  Main sequence routine.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    /* initialise helpers once */
    if (cache == NULL) {
        init_small_primes();
        ensure_cache_capacity(n);
    }

    /* generate terms until we have at least n terms */
    idx_t next_k = 0;
    if (cache_size > 0) next_k = (idx_t)cache[cache_size - 1] + 1;

    const idx_t BLOCK = 1024;   /* enumeration block size */

    while (cache_size < (size_t)n) {
        for (idx_t k = next_k;
             k < next_k + BLOCK && cache_size < (size_t)n;
             ++k)
        {
            uint64_t val = 181ULL * k + 179ULL;
            if (is_prime_uint64(val)) {
                ensure_cache_capacity(cache_size + 1);
                cache[cache_size++] = (seq_t)k;
            }
        }
        next_k += BLOCK;
    }

    return cache[n - 1];
}
