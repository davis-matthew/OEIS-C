/*
 * A074851  Numbers k such that k and k+1 both have exactly 2 distinct prime
 *          factors.
 * https://oeis.org/A074851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 6608, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 6938, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074851 – a(n) is the nth integer k such that
   the number of distinct prime factors of k and of (k+1) are both 2.
   The function searches for the required k by scanning positive
   integers and testing their distinct prime factor counts. */

/* Standard integer types and memory allocation */
#include <stdint.h>
#include <stdlib.h>

/* ---- Prime handling ---------------------------------------------------- */

/* dynamic array of primes */
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;       /* number of primes stored   */
static uint32_t prime_limit = 0;       /* largest number considered */

/* Simple sieve that fills prime_list with all primes <= limit.
   It reallocates prime_list if necessary. */
static void ensure_primes_upto(uint32_t limit)
{
    if (limit <= prime_limit) return;          /* already have enough */

    /* Allocate a temporary boolean array for the sieve */
    uint8_t *comp = (uint8_t*)calloc(limit + 1, sizeof(uint8_t));
    if (!comp) return;                         /* out of memory – just bail */

    /* Estimate number of primes to allocate space for */
    size_t est = limit / 10 + 10;
    uint32_t *new_list = (uint32_t*)realloc(prime_list, est * sizeof(uint32_t));
    if (!new_list) { free(comp); return; }     /* out of memory – bail */
    prime_list = new_list;

    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            /* store prime */
            if (cnt >= est) {
                est <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(prime_list, est * sizeof(uint32_t));
                if (!tmp) { free(comp); return; }
                prime_list = tmp;
            }
            prime_list[cnt++] = i;

            /* sieve multiples if possible */
            if ((uint64_t)i * i <= limit) {
                for (uint32_t j = i * i; j <= limit; j += i) comp[j] = 1;
            }
        }
    }
    prime_cnt   = cnt;
    prime_limit = limit;
    free(comp);
}

/* ---- count distinct prime factors ------------------------------------- */

/* Count how many distinct prime factors a number has.
   Stops early if more than 2 are found. */
static uint8_t distinct_prime_factor_count(uint64_t x)
{
    uint8_t cnt = 0;
    /* ensure we have primes up to sqrt(x) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > x) break;
    }
    /* if the last prime in the list is still <= sqrt(x), extend */
    while (prime_cnt > 0 && (uint64_t)prime_list[prime_cnt-1] * prime_list[prime_cnt-1] <= x) {
        ensure_primes_upto(prime_limit * 2 + 10);
    }
    /* factor using the generated primes */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) {
            ++cnt;
            if (cnt > 2) return 3;          /* early exit */
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++cnt;                       /* remaining prime factor > sqrt */
    return cnt;
}

/* ---- sequence implementation ------------------------------------------- */

/* Cache of computed terms */
static seq_t  *cache      = NULL;  /* array holding terms */
static size_t  cache_len  = 0;     /* how many terms stored */
static size_t  cache_cap  = 0;     /* allocated capacity */
static uint64_t next_candidate = 2; /* smallest integer not yet considered */

seq_t a(idx_t n)
{
    /* ensure a basic prime list is ready */
    if (prime_limit == 0) ensure_primes_upto(1000);

    /* return from cache if already computed */
    if (n <= (idx_t)cache_len) return cache[n-1];

    /* grow cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        for (uint64_t k = next_candidate; ; ++k) {
            if (distinct_prime_factor_count(k)   == 2 &&
                distinct_prime_factor_count(k+1) == 2) {
                cache[cache_len++] = (seq_t)k;
                next_candidate = k + 1;
                break;
            }
        }
    }
    return cache[n-1];
}
