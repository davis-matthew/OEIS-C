/*
 * A028505  Number of primes <= 100*n.
 * https://oeis.org/A028505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(100*n) – the number of primes ≤ 100·n
 * Range:     u8 n <= 16, u16 n <= 8216, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Cached prime table */
static uint64_t *prime_table = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of primes in the array */
static idx_t     curr_limit  = 0;      /* highest limit that was sieved */

/* Return the number of primes ≤ 100·n. */
seq_t a(idx_t n)
{
    /* handle n==0 defensively (offset is 1) */
    if (n == 0) return (seq_t)0;

    /* compute limit = 100 * n, cast to idx_t before multiplication */
    idx_t limit = n * (idx_t)100;

    /* extend sieve if necessary */
    if (limit > curr_limit) {
        /* free old table if it exists */
        if (prime_table != NULL)
            free(prime_table);

        /* build primes up to the new limit */
        build_primes_upto(limit, &prime_table, &prime_cnt);
        curr_limit = limit;
    }

    /* binary search for the first prime > limit */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_table[mid] <= limit)
            lo = mid + 1;
        else
            hi = mid;
    }

    /* lo is the count of primes ≤ limit */
    return (seq_t)lo;
}
