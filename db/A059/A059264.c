/*
 * A059264  Primes p such that x^12 = 2 has no solution mod p.
 * https://oeis.org/A059264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 4903, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd64, next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^12 ≡ 2 (mod p) has no solution
   Computation: iterate through primes, test the solvability
   condition 2^((p-1)/gcd(12,p-1)) ≡ 1 (mod p).  Return the n‑th
   prime for which the condition fails. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd64.h"
#include "../utils/powmod64.h"
#include "../utils/next_prime.h"

/* Static cache of qualifying primes. */
static seq_t  *cache   = NULL;   /* array of stored primes          */
static size_t  cache_len    = 0;   /* number of stored primes         */
static size_t  cache_cap    = 0;   /* current capacity of the array   */
static uint64_t last_checked = 1;   /* last prime examined (start → 1) */

/* Check whether a prime p satisfies the condition of the sequence. */
static bool p_has_no_solution(uint64_t p)
{
    /* For odd prime p, a solution to x^12 ≡ 2 exists iff
       2^((p-1)/gcd(12,p-1)) ≡ 1 (mod p).  Thus no solution when
       the above congruence fails. */
    uint64_t g   = gcd64(12ULL, p - 1ULL);
    uint64_t exp = (p - 1ULL) / g;
    return powmod64(2ULL, exp, p) != 1ULL;
}

/* Add a new qualifying prime p to the cache. */
static void add_prime(uint64_t p)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);   /* allocation failed */
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)p;
}

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Populate the cache until it contains at least n elements. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked);   /* next prime ≥ last_checked */
        last_checked = p;
        if (p != 2 && p_has_no_solution(p)) {
            add_prime(p);
        }
    }
    return cache[(size_t)(n - 1ULL)];
}
