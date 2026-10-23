/*
 * A102605  Number of ways of writing 2n+1 as p+q+r where p,q,r are primes
 *          with p < q < r, offset=0.
 * https://oeis.org/A102605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of ways to express 2*n+1 as p+q+r with distinct
 *            primes satisfying p < q < r
 * Range:     u8 n <= 190, u16 n <= 6520, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/simple_sieve.h"
#include "../utils/is_prime.h"

/* Cached prime list (sorted).  The list is rebuilt when a larger upper bound
   is required. */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static uint64_t prime_max   = 0;

/* Build the prime list up to at least 'target'.  The build is performed
   lazily and stored for future calls. */
static void ensure_primes(uint64_t target)
{
    if (target <= prime_max)
        return;                     /* nothing to do */

    /* free previous list if any */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;

    /* limit for the sieve must fit in uint32_t for the simple_sieve
       implementation used in the utils folder.  The tests use n <= 10000
       so this is safe. */
    uint32_t limit = (uint32_t)target;
    uint32_t *tmp   = NULL;
    size_t    tmpcnt = 0;

    simple_sieve(limit, &tmp, &tmpcnt);

    /* copy primes to a uint64_t array so that is_prime can be called with
       64‑bit values.  The cast to (uint64_t*) is required for C++ but is
       legal in C. */
    prime_list = (uint64_t *)malloc(tmpcnt * sizeof(uint64_t));
    for (size_t i = 0; i < tmpcnt; ++i)
        prime_list[i] = (uint64_t)tmp[i];

    prime_cnt = tmpcnt;
    prime_max = (uint64_t)limit;

    free(tmp);                     /* simple_sieve allocates this buffer */
}

seq_t a(idx_t n)
{
    /* The target sum is 2*n + 1. */
    uint64_t target = (uint64_t)2 * (uint64_t)n + 1;

    /* Make sure we have all primes up to 'target'. */
    ensure_primes(target);

    seq_t count = 0;

    /* Enumerate all distinct triples p < q < r with p+q+r == target. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > target / 3)           /* p too large for any triple */
            break;

        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t q = prime_list[j];
            uint64_t r = target - p - q;

            if (r <= q)                /* ensures p < q < r */
                break;

            if (is_prime(r))
                count++;               /* found a valid triple */
        }
    }

    return count;
}
