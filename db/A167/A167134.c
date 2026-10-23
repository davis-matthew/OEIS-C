/*
 * A167134  Primes congruent to {2, 3, 5, 7} mod 11.
 * https://oeis.org/A167134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to {2,3,5,7} mod 11.
   a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7, a(5) = 13, ...   */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

static uint64_t *prime_cache = NULL;      /* primes found so far          */
static size_t   prime_cnt   = 0;          /* number of primes in cache    */
static size_t   prime_cap   = 0;          /* capacity of prime_cache      */

static seq_t   *seq_cache   = NULL;      /* sequence terms (primes)       */
static size_t   seq_cnt     = 0;          /* number of terms stored       */
static size_t   seq_cap     = 0;          /* capacity of seq_cache        */

static uint64_t candidate   = 2;          /* current number to test       */

/* Append a value to a dynamic array of seq_t.  */
static void seq_expand(void) {
    if (seq_cap == 0) {
        seq_cap = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_cnt == seq_cap) {
        seq_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
}

/* Append a value to a dynamic array of uint64_t.  */
static void prime_expand(void) {
    if (prime_cap == 0) {
        prime_cap = 64;
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt == prime_cap) {
        prime_cap <<= 1;
        prime_cache = (uint64_t *)realloc(prime_cache, prime_cap * sizeof(uint64_t));
    }
}

/* Test whether x is prime using the primes already found.  */
static bool is_prime_candidate(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure that the sequence contains at least 'n' terms.  */
static void ensure_sequence(idx_t n) {
    size_t target = (size_t)n;
    if (seq_cnt >= target) return;

    while (seq_cnt < target) {
        /* Test current candidate for primality. */
        bool is_prime = is_prime_candidate(candidate);
        if (is_prime) {
            /* Store the prime. */
            prime_expand();
            prime_cache[prime_cnt++] = candidate;

            /* If it matches the required residue class, store in the sequence. */
            uint64_t rem = candidate % 11;
            if (rem == 2 || rem == 3 || rem == 5 || rem == 7) {
                seq_expand();
                seq_cache[seq_cnt++] = (seq_t)candidate;
            }
        }
        ++candidate;
    }
}

seq_t a(idx_t n) {
    /* Indexing starts at 1.  */
    if (n == 0) return (seq_t)0;    /* undefined by definition; return 0. */
    ensure_sequence(n);
    return seq_cache[(size_t)n - 1];
}
