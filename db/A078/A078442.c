/*
 * A078442  a(p) = a(n) + 1 if p is the n-th prime, prime(n); a(n)=0 if n is
 *          not prime.
 * https://oeis.org/A078442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 0 if n is composite; otherwise a(n) = a(k)+1 where
   k is the 1‑based index of n in the list of primes. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;   /* primes up to primes_limit */
static size_t prime_cnt  = 0;
static idx_t  primes_limit = 0;

/* Ensure that prime_list contains all primes <= limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit)
        return;
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_cnt);
    primes_limit = limit;
}

/* Return the 1‑based index of a prime p in prime_list.  p must be prime. */
static idx_t prime_index(idx_t p)
{
    idx_t lo = 0, hi = prime_cnt; /* [lo, hi) */
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* prime_list[lo] == p */
    return lo + 1; /* 1‑based */
}

seq_t a(idx_t n)
{
    /* Ensure prime list covers n. */
    ensure_primes_upto(n);

    seq_t count = 0;
    idx_t cur = n;
    while (is_prime(cur)) {
        ++count;
        cur = prime_index(cur);
    }
    return count;
}
