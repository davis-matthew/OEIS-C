/*
 * A038923  Primes p such that 43 is a square mod p.
 * https://oeis.org/A038923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime p for which 43 is a quadratic residue modulo p.
   The algorithm searches primes in order, tests the Legendre symbol (43/p),
   and collects those primes into a cache.  It memoises any previous results
   so that calls may arrive in any order. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"

/* Dynamic array helpers (generic) */
static void grow_array(size_t *cap, size_t min_cap, void **ptr, size_t elem_size) {
    if (*cap < min_cap) {
        size_t new_cap = *cap ? *cap * 2 : 64;
        while (new_cap < min_cap) new_cap <<= 1;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Prime list for trial division */
static uint64_t *prime_list = NULL;
static size_t prime_cap  = 0;
static size_t prime_cnt  = 0;

/* Cache of sequence values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_cnt  = 0;

/* Next odd number to test (starting at 3) */
static uint64_t candidate = 3;

/* Simple primality test using stored primes */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ull) == 0) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_list[i];
        if (q > n / q) break;          /* q > sqrt(n) */
        if (n % q == 0) return false;
    }
    return true;
}

/* Ensure that cache contains at least n terms. */
static void need_cache(idx_t n) {
    /* Seed the prime list with 2 if it is still empty. */
    if (prime_cnt == 0) {
        grow_array(&prime_cap, 1, (void **)&prime_list, sizeof(uint64_t));
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    while (cache_cnt < n) {
        if (is_prime_uint64(candidate)) {
            /* Store candidate as a prime for future divisibility tests. */
            grow_array(&prime_cap, prime_cnt + 1, (void **)&prime_list, sizeof(uint64_t));
            prime_list[prime_cnt++] = candidate;

            /* Check the quadratic-residue condition. */
            if (candidate == 43 || jacobi_symbol(43, candidate) == 1) {
                grow_array(&cache_cap, cache_cnt + 1, (void **)&cache, sizeof(seq_t));
                cache[cache_cnt++] = (seq_t)candidate;
            }
        }
        /* Advance to the next odd candidate. */
        candidate += 2;
        /* Overflow guard – unlikely for the ranges of interest. */
        if (candidate < 3) candidate = 3;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for 0, return 0 */
    need_cache(n);
    return cache[n - 1];
}
