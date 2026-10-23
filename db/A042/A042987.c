/*
 * A042987  Primes congruent to {2, 3, 5, 7} mod 8.
 * https://oeis.org/A042987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 4928, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042987: primes congruent to {2,3,5,7} modulo 8
   (i.e., primes p such that p % 8 != 1).  The function a(n) returns
   the n‑th such prime, where a(1)=2.  The implementation uses a simple
   incremental sieve: primes already found are stored in `all_primes`,
   and a candidate is tested for primality by trial division against
   that list.  Qualified primes are cached in `qual_cache`. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- static storage for caching ---------- */
static seq_t *qual_cache = NULL;   /* primes with residue != 1 (mod 8)   */
static size_t qual_cap   = 0;      /* current capacity of qual_cache     */
static size_t qual_len   = 0;      /* number of cached qualified primes */

static uint64_t *all_primes = NULL;/* all primes discovered so far       */
static size_t   all_cap     = 0;    /* current capacity of all_primes     */
static size_t   all_len     = 0;    /* number of primes discovered        */

static uint64_t current = 1;       /* next candidate to test (starts at 2) */

/* ---------- helper functions ---------- */
static void grow_qual_cap(size_t min_cap)
{
    if (qual_cap < min_cap) {
        size_t new_cap = qual_cap ? qual_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        qual_cache = (seq_t *)realloc(qual_cache, new_cap * sizeof(seq_t));
        qual_cap   = new_cap;
    }
}

static void grow_all_cap(size_t min_cap)
{
    if (all_cap < min_cap) {
        size_t new_cap = all_cap ? all_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        all_primes = (uint64_t *)realloc(all_primes, new_cap * sizeof(uint64_t));
        all_cap    = new_cap;
    }
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; n == 0 returns 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Ensure the cache contains at least n qualified primes. */
    while (qual_len < n) {
        /* Test the next integer for primality. */
        uint64_t candidate = current + 1;
        current = candidate;

        bool is_prime = true;
        for (size_t i = 0; i < all_len; ++i) {
            uint64_t p = all_primes[i];
            if (p > candidate / p) {   /* p*p > candidate, stop */
                break;
            }
            if ((candidate % p) == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the newly discovered prime. */
            if (all_len == all_cap) {
                grow_all_cap(all_cap ? all_cap * 2 : 16);
            }
            all_primes[all_len++] = candidate;

            /* Accept it if its residue modulo 8 is not 1. */
            if ((candidate % 8) != 1) {
                if (qual_len == qual_cap) {
                    grow_qual_cap(qual_cap ? qual_cap * 2 : 16);
                }
                qual_cache[qual_len++] = (seq_t)candidate;
            }
        }
    }

    return qual_cache[n - 1];   /* indices start at 1 */
}
