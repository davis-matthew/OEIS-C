/*
 * A215370  Primes congruent to {0, 2, 3, 5, 6} mod 11.
 * https://oeis.org/A215370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p mod 11 ∈ {0,2,3,5,6}
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *prime_cache = NULL;   /* cached terms of the sequence    */
static size_t cache_cap = 0;
static idx_t  cache_cnt  = 0;        /* number of terms cached         */

static uint64_t *prime_list = NULL; /* list of all primes discovered  */
static size_t   primes_cap  = 0;
static idx_t    primes_cnt  = 0;     /* number of primes stored        */

static idx_t next_candidate = 1;     /* next number to test for primality */

/* Grow the array that holds the cached sequence terms */
static void grow_cache(size_t needed)
{
    if (cache_cnt + needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (cache_cnt + needed > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Grow the array that holds the list of all primes found */
static void grow_primes(size_t needed)
{
    if (primes_cnt + needed > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        while (primes_cnt + needed > new_cap)
            new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);
        prime_list = tmp;
        primes_cap = new_cap;
    }
}

/* Test whether x is prime using the current list of primes */
static bool is_prime_u64(idx_t x)
{
    if (x < 2) return false;
    for (idx_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Return the n‑th term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    while (cache_cnt < n) {
        ++next_candidate;

        /* Skip even numbers except 2 */
        if (next_candidate > 2 && (next_candidate & 1) == 0)
            continue;

        if (is_prime_u64(next_candidate)) {
            grow_primes(1);
            prime_list[primes_cnt++] = (uint64_t)next_candidate;

            idx_t r = next_candidate % 11;
            if (r == 0 || r == 2 || r == 3 || r == 5 || r == 6) {
                grow_cache(1);
                prime_cache[cache_cnt++] = (seq_t)next_candidate;
                if (cache_cnt >= n)
                    return prime_cache[n - 1];
            }
        }
    }
    return prime_cache[n - 1];
}
