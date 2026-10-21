/*
 * A204892  Least k such that n divides s(k)-s(j) for some j in [1,k), where
 *          s(k)=prime(k).
 * https://oeis.org/A204892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 810, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204892:  a(n) = smallest k such that ∃ j<k with n | prime(k)-prime(j) */
/* (prime(k) = the k-th prime, offset 1) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache for primes */
static uint64_t *prime_cache = NULL;     /* array of primes */
static size_t   prime_count = 0;         /* number of primes stored */
static size_t   prime_capacity = 0;      /* allocated capacity */
static uint64_t last_checked = 3;        /* next odd candidate to test */

/* Ensure at least `need` primes are stored in prime_cache */
static void
ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* Allocate more space if necessary */
    if (prime_capacity < need) {
        size_t new_cap = (prime_capacity < 16) ? 16 : prime_capacity * 2;
        while (new_cap < need) new_cap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_capacity = new_cap;
    }

    /* Seed the cache if empty */
    if (prime_count == 0) {
        prime_cache[0] = 2;
        prime_count = 1;
        last_checked = 3;
    }

    /* Generate primes until we have `need` of them */
    while (prime_count < need) {
        bool is_prime = true;
        uint64_t cand = last_checked;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_count++] = cand;
        }
        last_checked += 2;  /* test only odd numbers */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined case */

    /* n = 1: any difference works, smallest k is 2 */
    if (n == 1) return (seq_t)2;

    /* We need at most n+1 primes: by the pigeonhole principle a collision
       must occur among the first n+1 primes. */
    size_t need = (size_t)n + 1;
    ensure_primes(need);

    /* Track residues modulo n */
    int *seen = (int *)calloc((size_t)n, sizeof(int));
    if (!seen) abort();

    /* residue of the first prime */
    uint64_t r = prime_cache[0] % (uint64_t)n;
    seen[(size_t)r] = 1;

    for (size_t k = 2; k <= need; ++k) {
        r = prime_cache[k-1] % (uint64_t)n;
        if (seen[(size_t)r]) {
            free(seen);
            return (seq_t)k;
        }
        seen[(size_t)r] = 1;
    }

    /* Should never happen: fallback */
    free(seen);
    return (seq_t)need;
}
