/*
 * A020614  Smallest nonempty set S containing prime divisors of 7k+10 for
 *          each k in S.
 * https://oeis.org/A020614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth smallest prime in the unique minimal nonempty set S
 *            with property: for each k in S, all prime divisors of 7k+10
 *            belong to S. The set S consists of 13 primes: 3, 11, 13, 17, 29,
 *            31, 41, 43, 71, 101, 227, 239, 311
 * Range:     u8 n <= 12, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   sort_list
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sort_list.h"

/* Helper: add a prime to the global list if not already present */
static void add_prime(uint64_t *list, size_t *count, uint64_t p)
{
    for (size_t i = 0; i < *count; ++i)
        if (list[i] == p)
            return;
    if (*count < 128)
        list[(*count)++] = p;
}

/* Helper: factor n into distinct prime divisors, adding them to list */
static void factor_and_add(uint64_t *list, size_t *count, uint64_t n)
{
    if (n % 2 == 0) {
        add_prime(list, count, 2);
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            add_prime(list, count, d);
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1)
        add_prime(list, count, n);
}

/* Global cached set */
static uint64_t cached_primes[128];
static size_t cached_count = 0;
static int cached_ready = 0;

/* Compute the minimal closed set once */
static void compute_cached_set(void)
{
    if (cached_ready)
        return;
    cached_count = 0;

    /* start with the smallest prime that can appear */
    add_prime(cached_primes, &cached_count, 3);

    size_t proc = 0;
    while (proc < cached_count) {
        uint64_t p = cached_primes[proc++];
        uint64_t x = 7 * p + 10;
        factor_and_add(cached_primes, &cached_count, x);
    }

    sort_list(cached_primes, cached_count);
    cached_ready = 1;
}

/* The required sequence function */
seq_t a(idx_t n)
{
    compute_cached_set();
    if ((idx_t)cached_count < n)
        return (seq_t)0;            /* out of bounds – not used in tests */
    return (seq_t)cached_primes[(size_t)(n - 1)];
}
