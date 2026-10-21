/*
 * A073881  a(n) = smallest number m (obviously prime) such that pi(m) =
 *          2*pi(n).
 * https://oeis.org/A073881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_{2 * pi(n)} (the 2*π(n)-th prime), with a(1)=1
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   upper_bound_prime_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code builds a dynamic sieve of Eratosthenes to produce primes up
   to the required bound.  The prime counting function π(n) is obtained
   by a binary search over the sorted prime list.  The sieve is grown
   lazily when a larger bound is needed.  Only standard C headers are
   used, so the function is valid in both C11 and C++17 builds. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/upper_bound_prime_index.h"

/* static state */
static unsigned char *is_composite = NULL;   /* 0 = prime, 1 = composite */
static size_t comp_capacity = 0;
static uint64_t *prime_list = NULL;          /* primes in order */
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static uint64_t max_limit = 0;               /* current sieve limit */

/* rebuilds the sieve up to 'limit' and repopulates prime_list */
static void rebuild_sieve(uint64_t limit)
{
    if (is_composite) {
        free(is_composite);
    }
    /* allocate composite array: +1 for index 0..limit */
    comp_capacity = (size_t)limit + 1;
    is_composite = (unsigned char *)calloc(comp_capacity, 1);

    if (prime_list) {
        free(prime_list);
    }
    /* initial capacity; grows as needed */
    prime_cap = (size_t)(limit / 10 + 100);
    prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    prime_cnt = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }
    max_limit = limit;
}

/* ensures that the sieve covers at least 'limit' */
static void ensure_limit(uint64_t limit)
{
    if (limit <= max_limit) return;
    /* simple strategy: double the limit until we reach desired bound */
    uint64_t new_limit = (limit > max_limit) ? limit : max_limit * 2;
    while (new_limit < limit) {
        new_limit *= 2;
    }
    rebuild_sieve(new_limit);
}

/* number of primes <= x via binary search over prime_list */
static size_t pi_of(uint64_t x)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;  /* count of primes <= x */
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 1) /* special case as per OEIS definition */
        return (seq_t)1;

    /* lazily initialize sieve */
    static bool started = false;
    if (!started) {
        rebuild_sieve(100ULL);  /* small initial sieve */
        started = true;
    }

    uint64_t un = (uint64_t)n;

    /* ensure sieve covers n to compute pi(n) */
    ensure_limit(un);

    size_t pi_n = pi_of(un);
    size_t target_index = pi_n * 2;  /* desired prime index (1-based) */

    if (target_index == 0)  /* should not happen except for n=1 handled above */
        return (seq_t)1;

    /* ensure enough primes are available */
    if (target_index > prime_cnt) {
        size_t new_limit = upper_bound_prime_index(target_index);
        if (new_limit <= max_limit)
            new_limit = max_limit * 2 + 100;
        rebuild_sieve(new_limit);
    }

    uint64_t result = prime_list[target_index - 1];  /* 0-based indexing */
    return (seq_t)result;
}
