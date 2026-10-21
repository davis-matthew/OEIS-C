/*
 * A053050  a(n) = smallest integer m such that Sum_{k=1..m} prime(k) is
 *          divisible by n.
 * https://oeis.org/A053050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 2131, u32 n <= 8997, u64 n <= 8997,
 *            u128 n <= 8997, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest integer m such that the sum of the
 * first m primes is divisible by n.  The function is memoised by
 * keeping a growing list of primes so that repeated calls are fast
 * and independent of ordering.  The algorithm is a simple
 * “incremental search” that never fails for n ≥ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static uint32_t *prime_list   = NULL;   /* stored primes */
static size_t   prime_count   = 0;      /* number of primes stored */
static size_t   prime_capacity = 0;    /* allocated size of prime_list */

/* Grow the prime list to hold at least 'need' elements.  The cast from
 * void* to uint32_t* is required for C++ compilation. */
static void grow_prime_list(size_t need)
{
    if (need <= prime_capacity) return;
    size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
    if (!tmp) exit(EXIT_FAILURE);
    prime_list = tmp;
    prime_capacity = new_cap;
}

/* We consider 2 as the first prime.  All other candidates are odd. */
static bool is_candidate_prime(uint32_t cand)
{
    if (cand == 2) return true;
    if ((cand & 1u) == 0) return false;
    uint32_t limit = (uint32_t)floor_sqrt((seq_t)cand);
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if (p > limit) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Return the least m such that Σ_{k=1..m} prime(k) ≡ 0 (mod n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, safe return */
    seq_t mod_n = (seq_t)n;
    seq_t rem   = 0;                     /* running sum modulo n */
    idx_t m     = 0;                     /* number of primes summed */
    uint32_t candidate = 2;              /* current candidate prime */

    for (;;) {
        if (!is_candidate_prime(candidate))
            { ++candidate; continue; }

        /* Store the new prime and update the running sum. */
        grow_prime_list(prime_count + 1);
        prime_list[prime_count++] = candidate;

        ++m;
        rem = (rem + (seq_t)candidate) % mod_n;
        if (rem == 0) return (seq_t)m;

        ++candidate;
    }
}
