/*
 * A019394  Primes with primitive root 71.
 * https://oeis.org/A019394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A019394: Primes that have 71 as a primitive root.
 * Computed by trial‑division for primality and repeated squaring
 * for the primitive‑root condition.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/* Helpers for dynamic arrays ------------------------------------------------- */

static void grow_if_needed(size_t *cnt, size_t *cap, seq_t **arr)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

/* State for primes ----------------------------------------------------------- */

static seq_t *all_primes     = NULL;   /* all primes found so far          */
static size_t all_prime_cnt  = 0;
static size_t all_prime_cap  = 0;

static seq_t *prim_prime      = NULL;   /* primes with primitive root 71   */
static size_t prim_cnt        = 0;
static size_t prim_cap        = 0;

static uint64_t next_candidate = 3;      /* next odd number tested */

/* Test primality using the list of previously found primes ------------------ */

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (size_t i = 0; i < all_prime_cnt; ++i) {
        uint64_t p = (uint64_t)all_primes[i];
        if (p * p > n) break;
        if (n % p == 0ULL) return false;
    }
    return true;
}

/* Test whether 71 is a primitive root modulo prime p ----------------------- */

static bool is_primitive_root_71(uint64_t p)
{
    if (p == 2ULL) return true;           /* 1 generates the unit group mod 2 */
    if (p == 71ULL) return false;         /* 71 ≡ 0 (mod 71) → never a root    */

    uint64_t phi = p - 1ULL;
    uint64_t tmp = phi;

    for (size_t i = 0; i < all_prime_cnt &&
                    (uint64_t)all_primes[i] * (uint64_t)all_primes[i] <= tmp; ++i) {
        uint64_t q = (uint64_t)all_primes[i];
        if (tmp % q == 0ULL) {
            if (mod_pow((seq_t)71, phi / q, (seq_t)p) == (seq_t)1) return false;
            while (tmp % q == 0ULL) tmp /= q;
        }
    }
    if (tmp > 1ULL) {
        if (mod_pow((seq_t)71, phi / tmp, (seq_t)p) == (seq_t)1) return false;
    }
    return true;
}

/* Main public function ------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for indices < 1 */

    /* One‑time initialization of the prime lists --------------------------- */
    if (all_prime_cnt == 0) {
        all_prime_cap = 16;
        all_primes = (seq_t *)malloc(all_prime_cap * sizeof(seq_t));
        if (!all_primes) abort();
        all_primes[0] = (seq_t)2;
        all_prime_cnt = 1;

        prim_cap = 16;
        prim_prime = (seq_t *)malloc(prim_cap * sizeof(seq_t));
        if (!prim_prime) abort();
        prim_prime[0] = (seq_t)2;          /* 2 has primitive root 71 by definition */
        prim_cnt = 1;

        next_candidate = 3;
    }

    /* Generate primes until we have at least n with the property -------------
     *  The search is independent of the order of calls; the static state
     *  remembers already found primes and roots. --------------------------------
     */
    while (prim_cnt < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;               /* only odd candidates after 2 */

        if (!is_prime_uint64(cand)) continue;

        /* Store new prime for future factorisations */
        grow_if_needed(&all_prime_cnt, &all_prime_cap, &all_primes);
        all_primes[all_prime_cnt++] = (seq_t)cand;

        /* Check primitive‑root property and store if true */
        if (cand == 2ULL || is_primitive_root_71(cand)) {
            grow_if_needed(&prim_cnt, &prim_cap, &prim_prime);
            prim_prime[prim_cnt++] = (seq_t)cand;
        }
    }

    return prim_prime[n - 1];
}
