/*
 * A089581  a(n) = prime(2*n-1)*prime(2*n).
 * https://oeis.org/A089581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(2*n-1)*prime(2*n)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 3271, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates primes on demand using a simple incremental
 * primality test, caching the results in a dynamic array.  The sequence
 * is defined for n >= 1.  The function works for any order of calls and
 * compiles for all required integer types. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Static cache for primes – 0‑based array, prime(1) is at index 0    */
static uint64_t *prime_cache = NULL;
static size_t prime_cnt  = 0;   /* number of primes stored              */
static size_t prime_cap  = 0;   /* allocated capacity of prime_cache    */
static uint64_t probe    = 2;   /* next candidate for primality test   */

/* Ensure that the cache can hold at least 'cap' prime numbers.  If
 * necessary, it is reallocated to a larger capacity.  The new capacity
 * is the next power‑of‑two not smaller than 'cap', or at least 64. */
static void grow_prime_cache(size_t cap)
{
    size_t new_cap = prime_cap ? (prime_cap << 1) : 64;
    if (new_cap < cap) new_cap = cap;
    uint64_t *new_arr = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!new_arr) {
        /* Allocation failure – abort as this is unrecoverable for the
           sequence calculation. */
        exit(EXIT_FAILURE);
    }
    prime_cache = new_arr;
    prime_cap   = new_cap;
}

/* Test if a number >= 2 is prime using the primes already in the cache. */
static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t limit = (uint64_t)(sqrt((double)n));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > limit) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Ensure that at least 'need' primes (1‑based indexing) are stored. */
static void ensure_primes(idx_t need)
{
    while (prime_cnt < (size_t)need) {
        if (is_prime(probe)) {
            if (prime_cnt >= prime_cap) grow_prime_cache(prime_cap);
            prime_cache[prime_cnt++] = probe;
        }
        ++probe;
    }
}

/* Return the 1‑based prime number at position 'idx' (idx >= 1). */
static seq_t prime_at(idx_t idx)
{
    if (idx < 1) return 0;
    ensure_primes(idx);
    return (seq_t)prime_cache[idx - 1];
}

/* Main function requested by the spec. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */
    seq_t p1 = prime_at(2 * n - 1);           /* prime(2n-1) */
    seq_t p2 = prime_at(2 * n);               /* prime(2n)   */
    return p1 * p2;                          /* product     */
}
