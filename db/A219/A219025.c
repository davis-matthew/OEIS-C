/*
 * A219025  Number of primes p<n such that 6n-p and 6n+p are both prime.
 * https://oeis.org/A219025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of primes p < n such that 6n-p and 6n+p are prime
 * Range:     u8 n <= 8819, u16 n <= 8819, u32 n <= 8819, u64 n <= 8819,
 *            u128 n <= 8819, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: primality testing (provided by the build system) */
#include "../utils/is_prime.h"

/* Static cache of primes < current_limit + 1 */
static uint64_t *prime_cache = NULL;   /* stored primes */
static size_t     prime_cnt   = 0;     /* number of stored primes */
static size_t     prime_cap   = 0;     /* current capacity of prime_cache */
static idx_t      prime_last  = 0;     /* largest integer that has been examined */

/* Grow the prime cache (double its capacity or set to 16) */
static void
grow_cache(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *new_ptr = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_ptr) {
        /* Allocation failure – terminate the program */
        abort();
    }
    prime_cache = new_ptr;
    prime_cap = new_cap;
}

/* Ensure that all primes < limit are in prime_cache;
   primes >= limit are not considered. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_last) return;
    if (prime_last == 0) {
        /* initial seed: 2 is the first prime */
        if (prime_cap == 0) grow_cache();
        prime_cache[0] = 2;
        prime_cnt = 1;
        prime_last = 1;           /* we've examined 1 */
    }
    for (idx_t i = prime_last + 1; i < limit; ++i) {
        if (is_prime(i)) {
            if (prime_cnt == prime_cap) grow_cache();
            prime_cache[prime_cnt++] = (uint64_t)i;
        }
    }
    prime_last = limit - 1;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* Gather all primes p < n */
    ensure_primes(n);

    seq_t cnt = 0;
    idx_t base = 6 * n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p >= n) break;          /* safety: only p < n */

        idx_t t1 = base - (idx_t)p;
        idx_t t2 = base + (idx_t)p;

        if (is_prime(t1) && is_prime(t2)) ++cnt;
    }

    return cnt;
}
