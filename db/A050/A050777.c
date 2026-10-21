/*
 * A050777  First occurrence from iterated procedure 'composite k added to sum
 *          of its prime factors reaches a prime' yielding n skipped primes.
 * https://oeis.org/A050777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A050777 – First occurrence from iterated procedure
 *  a(n) = the first composite k such that by repeatedly adding the sum of
 *         its prime factors one eventually reaches a prime p, and the
 *         number of primes strictly between k and p equals n.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----------------- global prime sieve --------------------------------- */
static uint32_t sieve_limit           = 0;          /* highest number sieved */
static unsigned char *isComposite     = NULL;        /* 0 = prime, 1 = composite */
static uint32_t *prime_list           = NULL;        /* primes in ascending order */
static size_t    primes_cnt           = 0;           /* number of primes in list */

/* ----------------- cache for sequence values --------------------------- */
static seq_t *cache      = NULL;      /* cache[n] = a(n) (0 if not yet found) */
static size_t cache_cap  = 0;          /* allocated size of cache */
static size_t cache_len  = 0;          /* maximum n found + 1 */

/* ----------------- next composite to test ------------------------------ */
static uint64_t next_k = 4;              /* first composite to start from */

/* ----------------- helper: build sieve -------------------------------- */
static void build_sieve(uint32_t limit)
{
    if (isComposite) free(isComposite);
    if (prime_list) free(prime_list);

    sieve_limit = limit;
    isComposite = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!isComposite) exit(EXIT_FAILURE);

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!isComposite[i]) {
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                isComposite[(size_t)j] = 1;
        }
    }
    isComposite[0] = isComposite[1] = 1;                         /* 0,1 not prime */

    /* build list of primes */
    prime_list = (uint32_t *)malloc((size_t)(limit + 1) * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    primes_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) prime_list[primes_cnt++] = i;
    if (primes_cnt)
        prime_list = (uint32_t *)realloc(prime_list, primes_cnt * sizeof(uint32_t));
}

/* ensure sieve extends to at least up_to (inclusive) */
static void ensure_limit(uint32_t up_to)
{
    if (up_to <= sieve_limit) return;
    uint32_t new_lim = sieve_limit ? sieve_limit * 2 : 8192;
    while (new_lim < up_to) new_lim <<= 1;
    build_sieve(new_lim);
}

/* ----------------- helper: prime counting ------------------------------ */
static size_t prime_count_le(uint32_t val)          /* primes <= val */
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ----------------- helper: sum of prime factors (with multiplicity) ----- */
static uint64_t sum_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)prime_list[i] * prime_list[i] <= temp; ++i) {
        uint32_t p = prime_list[i];
        while (temp % p == 0) {
            sum += p;
            temp /= p;
        }
    }
    if (temp > 1) sum += temp;     /* remaining prime factor */
    return sum;
}

/* ----------------- helper: resize cache -------------------------------- */
static void resize_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    memset(new_arr + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* ----------------- main routine ---------------------------------------- */
static void compute_until(idx_t target_n)
{
    /* initialise structures once */
    if (!cache) {
        cache = (seq_t *)calloc(128, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = 128;
    }
    if (sieve_limit == 0) {
        ensure_limit(8192);
        if (!prime_list) exit(EXIT_FAILURE);
    }

    /* iterate over composites until a(target_n) is found */
    while (target_n >= 0 && cache[(size_t)target_n] == 0) {
        uint64_t k = next_k++;
        if (k < 4) { k = 4; next_k = 5; }
        if (k > sieve_limit) ensure_limit((uint32_t)k);
        if (!isComposite[(size_t)k]) continue;          /* skip primes */

        /* iterative procedure: keep adding sum of prime factors until prime */
        uint64_t cur = k;
        while (true) {
            if (cur <= sieve_limit && !isComposite[(size_t)cur]) break;
            uint64_t sum = sum_prime_factors(cur);
            cur += sum;
            if (cur > sieve_limit) ensure_limit((uint32_t)cur);
        }
        uint64_t prime = cur;

        /* number of primes strictly between k and prime */
        size_t primes_le_k       = prime_count_le((uint32_t)k);
        size_t primes_le_primem1 = prime_count_le((uint32_t)(prime - 1));
        size_t n_skipped = primes_le_primem1 - primes_le_k;

        if (n_skipped >= cache_cap) resize_cache(n_skipped + 1);
        if (cache[(size_t)n_skipped] == 0) {
            cache[(size_t)n_skipped] = (seq_t)k;
            if (n_skipped >= cache_len) cache_len = n_skipped + 1;
        }
    }
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;        /* out of domain */
    compute_until(n);
    return cache[(size_t)n];
}
