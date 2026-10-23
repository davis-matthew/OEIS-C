/*
 * A023517  Greatest prime divisor of prime(n)*prime(n-1) - 1.
 * https://oeis.org/A023517
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 88, u32 n <= 6609, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the greatest prime divisor of prime(n)*prime(n-1)-1.
   Formula: a(n) = largest prime divisor of (p_n * p_{n-1}) - 1
   where p_k is the k-th prime (OEIS A000040). */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Cache of the first primes we have computed. */
static uint64_t *prime_cache = NULL;
static size_t    prime_cnt   = 0;
static idx_t     prime_limit = 0;

/* Ensure that at least n primes are available in prime_cache. */
static void
ensure_primes(idx_t n)
{
    if ((size_t)n <= prime_cnt) return;  /* already enough */

    /* Determine an upper bound for the n‑th prime. */
    idx_t new_limit = _prime_limit_for_idx(n);

    /* Allocate a new list of primes up to new_limit. */
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(new_limit, &new_primes, &new_cnt);

    /* Replace the old cache. */
    if (prime_cache) free(prime_cache);
    prime_cache = new_primes;
    prime_cnt   = new_cnt;
    prime_limit = new_limit;
}

/* Return greatest prime divisor of prime(n)*prime(n-1) - 1. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* offset is 2 */

    ensure_primes(n);                         /* primes up to n */

    uint64_t p_n   = prime_cache[n-1];        /* prime(n)   */
    uint64_t p_np1 = prime_cache[n-2];        /* prime(n-1) */

    /* Compute (p_n * p_{n-1}) - 1 in a type that can hold the product. */
    uint64_t prod  = p_n * p_np1;             /* 64‑bit product */
    seq_t val      = (seq_t)(prod - 1);        /* cast to seq_t */

    /* Factor val, keeping the largest prime factor seen. */
    seq_t largest = 1;
    seq_t temp    = val;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        seq_t pp   = (seq_t)p;
        seq_t p2   = pp * pp;
        if (p2 > temp) break;                 /* remaining factor is prime */

        if (temp % pp == 0) {
            while (temp % pp == 0) temp /= pp;
            largest = pp;                     /* new largest prime factor */
        }
    }

    if (temp > 1) largest = temp;             /* remaining prime factor */

    return largest;
}
