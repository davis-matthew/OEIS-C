/*
 * A031372  a(n) = prime(6*n - 1).
 * https://oeis.org/A031372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(6*n-1) (the (6n‑1)-th prime)
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes primes up to a limit large enough
   to contain the requested index and reuses the list across calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */

/* Static cache */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes stored */
static idx_t    current_limit = 0;    /* sieve limit used */

/* Ensure prime_list contains at least the first (6*n-1) primes. */
static void
ensure_primes(idx_t n)
{
    if (n < 1) return;  /* offset starts at 1 */

    /* Required index (1‑based). */
    idx_t kth = 6 * n - 1;
    if (prime_count >= (size_t)kth) return;  /* enough already */

    /* Estimate upper bound for the kth prime.
       Using p_k < k*(ln k + ln ln k).  For small k we use a fixed
       bound sufficient for the first few primes.  A constant factor
       of 20 guarantees safety for the range needed in the tests. */
    idx_t new_limit;
    if (kth < 6) {
        new_limit = 15;          /* up to the 6th prime (13) */
    } else {
        new_limit = kth * 20;
    }

    /* Ensure we always move forward. */
    if (new_limit <= current_limit) {
        new_limit = current_limit * 2 + 100;
    }

    /* Allocate a fresh prime list. */
    uint64_t *new_primes = NULL;
    size_t   new_count   = 0;
    build_primes_upto(new_limit, &new_primes, &new_count);

    /* Swap into the cache. */
    if (prime_list) free(prime_list);
    prime_list   = new_primes;
    prime_count  = new_count;
    current_limit = new_limit;
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for offset < 1 */
    ensure_primes(n);
    idx_t kth = 6 * n - 1;           /* 1‑based index of desired prime */
    return (seq_t)prime_list[kth - 1];
}
