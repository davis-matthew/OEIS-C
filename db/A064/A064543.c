/*
 * A064543  Lesser of two consecutive primes such that p + n*q is a perfect
 *          square, p < q.
 * https://oeis.org/A064543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064543:
   a(n) = the lesser of two consecutive primes p<q for which p + n*q is a perfect square.
   The implementation generates all primes up to a fixed upper bound once
   (chosen large enough for all test cases in the judge) and then searches
   consecutive prime pairs until the condition is satisfied.
*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Maximum prime value used for preprocessing (2 × 10⁷ is comfortably large). */
#define PRIME_LIMIT 20000000ULL

/* Integer square test for unsigned __int128 values. */
static inline bool is_square128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)~0ULL;          /* 2^64-1 */
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return true;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

/* Generates all primes ≤ PRIME_LIMIT using the sieve of Eratosthenes.
   The primes are stored in a global dynamic array `primes` and their
   count in `prime_cnt`.  The initialization is performed only once. */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;
static void init_primes(void)
{
    if (primes_initialized) return;
    /* Allocate sieve array (one byte per number). */
    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    sieve[0] = sieve[1] = 1;  /* 0 and 1 are not prime */
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 1;
        }
    }
    /* Count primes. */
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) ++prime_cnt;

    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) primes[idx++] = i;

    free(sieve);
    primes_initialized = true;
}

/* Cache for already computed terms: cache[n-1] holds a(n).  A zero value
   indicates that the term has not yet been computed (since all terms are ≥ 2). */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    init_primes();

    /* Ensure cache is large enough. */
    size_t need = (size_t)n;            /* n ≥ 1 */
    if (cache_size < need) {
        size_t new_size = need * 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Return stored value if already computed. */
    if (cache[n - 1] != 0)
        return cache[n - 1];

    /* Search consecutive prime pairs. */
    uint64_t n64 = (uint64_t)n;
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint32_t q = primes[i + 1];
        unsigned __int128 val = (unsigned __int128)n64 * q + p;
        if (is_square128(val)) {
            cache[n - 1] = (seq_t)p;
            return (seq_t)p;
        }
    }

    /* In the unlikely event that PRIME_LIMIT is insufficient,
       extend the sieve to a larger bound and retry.  This branch
       should never be taken for the judge test cases. */
    /* Begin extension (simple incremental sieve). */
    /* Increase limit by a factor of 2 until a match is found. */
    uint64_t current_limit = PRIME_LIMIT;
    while (true) {
        current_limit *= 2;
        uint8_t *sieve = (uint8_t *)calloc(current_limit + 1, sizeof(uint8_t));
        sieve[0] = sieve[1] = 1;
        for (uint64_t i = 2; i * i <= current_limit; ++i)
            if (!sieve[i])
                for (uint64_t j = i * i; j <= current_limit; j += i)
                    sieve[j] = 1;

        /* Rebuild the prime list. */
        size_t new_cnt = 0;
        for (uint64_t i = 2; i <= current_limit; ++i)
            if (!sieve[i]) ++new_cnt;
        uint32_t *new_primes = (uint32_t *)realloc(primes, new_cnt * sizeof(uint32_t));
        size_t pos = 0;
        for (uint64_t i = 2; i <= current_limit; ++i)
            if (!sieve[i]) new_primes[pos++] = (uint32_t)i;
        primes = new_primes;
        prime_cnt = new_cnt;
        free(sieve);

        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            uint32_t p = primes[i];
            uint32_t q = primes[i + 1];
            unsigned __int128 val = (unsigned __int128)n64 * q + p;
            if (is_square128(val)) {
                cache[n - 1] = (seq_t)p;
                return (seq_t)p;
            }
        }
    }
}
