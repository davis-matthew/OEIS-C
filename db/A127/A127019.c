/*
 * A127019  a(n) is the least semiprime s such that s - 2*n is also a
 *          semiprime.
 * https://oeis.org/A127019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A127019:  a(n) = least semiprime s such that s−2*n is also a semiprime. */
/*  The algorithm builds a table of semiprimes up to a fixed limit and, for each
    requested n, searches upward from s = 2*n + 4 until a suitable pair is found.
    Results are cached per n for repeated calls.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SEMI_PRIME_LIMIT ((idx_t)5000000)   /* ensure enough range for n ≤ 10⁴ */

static unsigned char *is_semiprime = NULL; /* 0/1 array */
static uint32_t *smallest_prime_factor = NULL;
static idx_t current_limit = 0;

/* Build smallest prime factor array and semiprime table up to LIMIT. */
static void build_semiprime_table(idx_t LIMIT) {
    /* allocate */
    smallest_prime_factor = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    is_semiprime = (unsigned char*)malloc((LIMIT + 1) * sizeof(unsigned char));
    if (!smallest_prime_factor || !is_semiprime) exit(1);

    smallest_prime_factor[0] = smallest_prime_factor[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) smallest_prime_factor[i] = 0;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (smallest_prime_factor[i] == 0) {              /* i is prime */
            smallest_prime_factor[i] = (uint32_t)i;
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (smallest_prime_factor[j] == 0)
                    smallest_prime_factor[j] = (uint32_t)i;
        }
    }
    /* remaining primes */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (smallest_prime_factor[i] == 0) smallest_prime_factor[i] = (uint32_t)i;

    /* build semiprime table */
    is_semiprime[0] = is_semiprime[1] = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        uint32_t p = smallest_prime_factor[i];
        idx_t  q  = i / p;
        if (q >= 2 && smallest_prime_factor[q] == (uint32_t)q)
            is_semiprime[i] = 1;         /* i = p*q with both primes */
        else
            is_semiprime[i] = 0;
    }

    current_limit = LIMIT;
}

/* Cache for computed a(n) values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* initialize tables once */
    if (current_limit == 0) build_semiprime_table(SEMI_PRIME_LIMIT);

    /* ensure cache capacity */
    if (n >= cache_size) {
        idx_t new_size = (cache_size == 0) ? 128 : cache_size;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        if (cache_size < new_size) {
            for (idx_t i = cache_size; i < new_size; ++i)
                cache[i] = 0;
        }
        cache_size = new_size;
    }

    /* return memoized value if present */
    if (cache[n] != 0) return cache[n];

    seq_t target = (seq_t)n * (seq_t)2;     /* 2*n */

    /* Search for smallest s ≥ 2*n + 4 that satisfies the condition */
    for (seq_t s = target + 4; (idx_t)s <= current_limit; ++s) {
        idx_t si = (idx_t)s;
        if (!is_semiprime[si]) continue;
        idx_t t_index = si - (idx_t)target;
        if (t_index >= 4 && is_semiprime[t_index]) {
            cache[n] = s;
            return s;
        }
    }

    /* If no solution found within limit, extend the table (unlikely for n ≤ 10⁴) */
    /* Double the limit and retry */
    idx_t new_limit = current_limit * 2;
    build_semiprime_table(new_limit);
    for (seq_t s = target + 4; (idx_t)s <= new_limit; ++s) {
        idx_t si = (idx_t)s;
        if (!is_semiprime[si]) continue;
        idx_t t_index = si - (idx_t)target;
        if (t_index >= 4 && is_semiprime[t_index]) {
            cache[n] = s;
            return s;
        }
    }
    /* If still not found, return 0 as an error indicator */
    return 0;
}
