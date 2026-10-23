/*
 * A102617  Primes prime(k) where k is a second-order nonprime number.
 * https://oeis.org/A102617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102617: primes whose index is a second‑order nonprime number.
   Compute the nth term by scanning natural numbers, counting the
   second‑order nonprime indices, and retrieving the corresponding
   prime.  Use a cached list of primes for efficiency. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Cached list of primes to accelerate kth_prime queries. */
static seq_t *prime_list     = NULL;
static size_t prime_count    = 0;
static size_t prime_capacity = 0;
static uint64_t prime_next   = 2; /* next integer to test for primality */

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* Allocate or grow the buffer. */
    if (prime_capacity == 0)
        prime_capacity = 128;
    while (prime_capacity < need) {
        size_t new_cap = prime_capacity * 2;
        if (new_cap < prime_capacity + 128)   /* avoid overflow of size_t */
            new_cap = prime_capacity + 128;
        prime_capacity = new_cap;
    }
    if (prime_list == NULL) {
        prime_list = (seq_t *)malloc(prime_capacity * sizeof(seq_t));
    } else {
        prime_list = (seq_t *)realloc(prime_list,
                                      prime_capacity * sizeof(seq_t));
    }
    /* Generate primes until we have 'need' of them. */
    while (prime_count < need) {
        if (is_prime(prime_next)) {
            prime_list[prime_count++] = (seq_t)prime_next;
        }
        ++prime_next;
    }
}

/* Return the k-th prime (k >= 1). */
static inline seq_t kth_prime(idx_t k)
{
    /* Prime k may be larger than prime_next-1 if k is new. */
    ensure_primes((size_t)k);
    return prime_list[(size_t)(k - 1)];
}

/* Main entry.  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; below offset is irrelevant */
        return (seq_t)0;

    idx_t found = 0;          /* number of second‑order nonprimes found */
    idx_t num   = 1;          /* current integer being examined */
    idx_t np_idx = 0;         /* index of current nonprime among all nonprimes */

    /* Scan integers until we locate the n‑th second‑order nonprime. */
    while (found < n) {
        if (!is_prime(num)) {          /* num is nonprime */
            ++np_idx;                 /* its index among nonprimes */
            if (!is_prime(np_idx)) {  /* index itself is nonprime => 2‑nd order */
                ++found;
                if (found == n)
                    return kth_prime(num);
            }
        }
        ++num;
    }
    /* Unreachable, keeps the compiler happy. */
    return (seq_t)0;
}
