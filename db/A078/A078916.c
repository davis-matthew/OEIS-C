/*
 * A078916  a(n) = prime(n) + 2*n.
 * https://oeis.org/A078916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 2*n
 * Range:     u8 n <= 40, u16 n <= 5541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Global cache for computed primes */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;        /* number of primes stored */
static size_t   prime_cap   = 0;        /* capacity of the array */

/* Test if x is a candidate prime using the current cache */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that at least n primes are stored in the cache */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n) return;   /* already have enough */

    if (prime_cap == 0) {
        prime_cap = 16;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate;
    if (prime_cnt == 0) {
        candidate = 2;
    } else {
        candidate = prime_cache[prime_cnt - 1] + 1;
    }

    while (prime_cnt < (size_t)n) {
        if (candidate <= 2) {
            candidate = 2;
        }
        /* Skip even numbers greater than 2 */
        if (candidate > 2 && (candidate & 1ULL) == 0) {
            ++candidate;
        }

        if (is_prime_candidate(candidate)) {
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                prime_cache = (uint64_t *)realloc(prime_cache,
                                                  prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Sequence definition */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */

    ensure_primes(n);

    seq_t prime_val = (seq_t)prime_cache[n - 1];
    seq_t add       = (seq_t)2 * (seq_t)n;
    return prime_val + add;
}
