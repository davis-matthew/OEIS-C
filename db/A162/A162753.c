/*
 * A162753  a(0)=a(1)=2; a(n) is the smallest prime such that a(n-1)^a(n) >
 *          a(n-2)^a(n-1).
 * https://oeis.org/A162753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 109, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A162753: 
 *   a(0)=a(1)=2; 
 *   for 2 ≤ n ≤ 5, a(n) is the smallest prime p such that a(n‑1)^p > a(n‑2)^a(n‑1). 
 *   for n ≥ 6, a(n) = prime(floor(n/2)).
 *
 * The implementation precomputes only the first 6 terms by the recurrence and
 * then uses the closed‑form formula, generating primes on demand.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/big_pow.h"

/* ------- small‑term cache (indices 0 … 5) ------- */
static seq_t small_cache[6];
static bool small_cache_inited = false;

/* ------- prime storage (used for the closed form) ------- */
#define PRIME_ARRAY_SIZE 6000   /* enough for prime(5000) etc. */
static seq_t primes[PRIME_ARRAY_SIZE];
static size_t primes_count = 0;    /* number of primes stored */
static idx_t next_prime_candidate = 3;  /* next odd number to test */

/* Ensure that primes[0 … k‑1] contain the first k primes (k ≥ 1). */
static void
ensure_primes_upto_idx(idx_t k)
{
    if (k == 0) return;
    while (primes_count <= (size_t)k) {          /* need at least k primes */
        if (primes_count == 0) {                 /* initialise with 2 */
            primes[0] = (seq_t)2;
            primes_count = 1;
            next_prime_candidate = 3;
        } else {
            idx_t cand = next_prime_candidate;
            while (!is_prime(cand)) cand += 2;   /* skip even numbers */
            primes[(size_t)primes_count] = (seq_t)cand;
            primes_count++;
            next_prime_candidate = (idx_t)cand + 2;
        }
    }
}

/* ------- main sequence function ------- */
seq_t
a(idx_t n)
{
    /* Initialise the small‑term cache on first use. */
    if (!small_cache_inited) {
        small_cache[0] = (seq_t)2;
        small_cache[1] = (seq_t)2;
        /* Compute terms 2 … 5 by the defining recurrence. */
        for (size_t i = 2; i <= 5; ++i) {
            seq_t prevprev = small_cache[i - 2];
            seq_t prev     = small_cache[i - 1];
            seq_t p = 2;
            while (true) {
                if (is_prime((idx_t)p)) {
                    seq_t lhs = big_pow(prev, p);
                    seq_t rhs = big_pow(prevprev, prev);
                    if (lhs > rhs) break;
                }
                p = (p == 2) ? 3 : p + 2;       /* next prime candidate */
            }
            small_cache[i] = p;
        }
        small_cache_inited = true;
    }

    /* Return cached values for n < 6. */
    if (n < 6) return small_cache[(size_t)n];

    /* For n ≥ 6 use the closed form a(n) = prime(floor(n/2)). */
    idx_t k = n / 2;                 /* floor(n/2) */
    ensure_primes_upto_idx(k);       /* guarantee at least k primes */
    return primes[(size_t)k - 1];    /* 0‑based indexing */
}
