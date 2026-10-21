/*
 * A224865  Numbers n such that 90*n + 89 and 90*n + 91 are twin primes.
 * https://oeis.org/A224865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224865 (OEIS) – numbers n such that 90*n+89 and 90*n+91 are twin primes.
   Implementation notes:
   - Uses a cache of qualifying n
   - Expands a prime sieve on demand via build_primes_upto
   - Computes candidates in increasing order, so the sequence is produced correctly
     regardless of call order. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x); */
#include "../utils/build_primes_upto.h" /* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) */

static uint64_t *found_cache = NULL; /* cache of valid n values */
static size_t   found_cap   = 0;
static size_t   found_len   = 0;

static uint64_t *prime_list = NULL; /* prime table used implicitly by is_prime */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;     /* largest number for which prime_list is valid */

static idx_t last_checked = 0;      /* highest candidate processed */

/* Ensure that prime_list covers all numbers up to limit. */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Append a new qualifying n to the cache. */
static void cache_push(uint64_t k)
{
    if (found_len == found_cap) {
        size_t new_cap = found_cap ? found_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
        if (!tmp)
            return; /* OOM – not expected in normal use */
        if (found_cache) {
            for (size_t i = 0; i < found_len; ++i)
                tmp[i] = found_cache[i];
            free(found_cache);
        }
        found_cache = tmp;
        found_cap   = new_cap;
    }
    found_cache[found_len++] = k;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* below offset – undefined */

    /* Expand cache until it contains at least n elements. */
    while (found_len < n) {
        ++last_checked;                         /* next candidate integer */
        uint64_t p1 = 90ULL * last_checked + 89ULL;
        uint64_t p2 = 90ULL * last_checked + 91ULL;

        /* Ensure prime list covers the larger of the two numbers. */
        idx_t need = p1 > p2 ? p1 : p2;
        ensure_prime_sieve(need);

        /* Test both numbers for primality. */
        if (is_prime(p1) && is_prime(p2))
            cache_push(last_checked);
    }

    return (seq_t)found_cache[n - 1];
}
