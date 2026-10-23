/*
 * A038609  Numbers that are the sum of 2 different primes.
 * https://oeis.org/A038609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 9225, u32 n <= 9225, u64 n <= 9225,
 *            u128 n <= 9225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th integer that can be written as the sum of
   two distinct primes.  It collects the terms using a simple sieve
   for primes up to an initial limit and then tests each candidate
   sequentially.  The sequence is memoised in a static cache so that
   subsequent calls are inexpensive.  The algorithm is O(n + m) where
   m is the value of the n‑th term.  The implementation uses only
   integer arithmetic and the given seq_t/idx_t types.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* -------------------  Prime generation  ------------------- */

static bool          *sieve_is_prime = NULL;   /* NULL → not yet built */
static uint64_t      *sieve_primes   = NULL;   /* list of primes */
static size_t        sieve_prime_cnt = 0;       /* number of primes */
static idx_t         sieve_limit     = 0;       /* current sieve size */

/* Initial limit large enough for n up to about 10 000          */
#define INITIAL_SIEVE_LIMIT 200000

static void build_prime_sieve(idx_t limit)
{
    if (sieve_is_prime != NULL)                /* already built */
        return;

    /* Allocate and initialise the sieve array.  */
    sieve_is_prime = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!sieve_is_prime) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i <= limit; ++i)
        sieve_is_prime[i] = true;

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (sieve_is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve_is_prime[j] = false;
        }
    }

    /* Collect primes into an array.  */
    sieve_primes = (uint64_t*)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!sieve_primes) exit(EXIT_FAILURE);

    sieve_prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve_is_prime[i])
            sieve_primes[sieve_prime_cnt++] = (uint64_t)i;

    sieve_limit = limit;
}

/* -------------------  Search helper  ------------------- */

static bool can_sum_two_distinct_primes(seq_t m)
{
    /* m is known to be small enough to fit in uint64_t for our limits. */
    uint64_t val = (uint64_t)m;

    for (size_t i = 0; i < sieve_prime_cnt; ++i) {
        uint64_t p = sieve_primes[i];
        if (p > val - 2)                     /* primes larger than m-2 give q < p */
            break;
        uint64_t q = val - p;
        if (q == p)                          /* distinct primes required */
            continue;
        if (q <= (uint64_t)sieve_limit) {
            if (sieve_is_prime[q])
                return true;
        } else
            return false;                     /* should not happen for current limit */
    }
    return false;
}

/* -------------------  Sequence cache  ------------------- */

static seq_t *seq_cache   = NULL;   /* dynamic array of sequence values */
static size_t cache_size  = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Grow the cache when needed.  */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_buf = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    seq_cache = new_buf;
    cache_cap = new_cap;
}

/* Build the cache up to at least n terms.  */
static void ensure_cache(idx_t n)
{
    /* Build the prime sieve once with the initial limit.  */
    if (sieve_is_prime == NULL)
        build_prime_sieve(INITIAL_SIEVE_LIMIT);

    while (cache_size < (size_t)n) {
        seq_t next_candidate;
        if (cache_size == 0)
            next_candidate = (seq_t)5;      /* the first term */
        else
            next_candidate = seq_cache[cache_size - 1] + (seq_t)1;

        /* Find next integer that is sum of two distinct primes. */
        while (!can_sum_two_distinct_primes(next_candidate))
            next_candidate = next_candidate + (seq_t)1;

        if (cache_size == cache_cap)
            grow_cache();

        seq_cache[cache_size++] = next_candidate;
    }
}

/* -------------------  Public function  ------------------- */

seq_t a(idx_t n)
{
    if (n == 0) {      /* undefined for offset 0, but return 0 */
        return (seq_t)0;
    }
    ensure_cache(n);
    return seq_cache[n - 1];
}
