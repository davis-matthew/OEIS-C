/*
 * A141171  Primes of the form -x^2+4*x*y+2*y^2 (as well as of the form
 *          5*x^2+8*x*y+2*y^2).
 * https://oeis.org/A141171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1640, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_match_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that p = 2 or p ≡ 5 or 23 (mod 24). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_match_prime.h"

/* Static storage for primes and matching primes. */
static uint64_t *prime_list   = NULL;
static size_t   prime_cnt     = 0;
static size_t   prime_cap     = 0;

static uint64_t *match_list   = NULL;
static size_t   match_cnt     = 0;
static size_t   match_cap     = 0;

static idx_t    current_limit = 0;  /* largest number already sieved */

/* Ensure capacity of prime_list. */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Ensure capacity of match_list. */
static void ensure_match_cap(size_t needed)
{
    if (match_cap < needed) {
        size_t new_cap = match_cap ? match_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(match_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        match_list = tmp;
        match_cap  = new_cap;
    }
}

/* Append a new prime to the stored lists. */
static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
    if (is_match_prime(p)) {
        ensure_match_cap(match_cnt + 1);
        match_list[match_cnt++] = p;
    }
}

/* Generate all primes up to `limit` (inclusive), extending
   current_limit and the prime / match lists.  Candidates are tested
   by trial division using the primes already discovered. */
static void generate_primes_up_to(idx_t limit)
{
    if (limit <= current_limit) return;          /* nothing more to do */

    for (idx_t cand = current_limit + 1; cand <= limit; ++cand) {
        if (cand < 2) continue;
        if ((cand & 1ULL) == 0ULL && cand != 2) continue;  /* skip even >2 */

        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > (uint64_t)cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime((uint64_t)cand);
    }
    current_limit = limit;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined offset, return 0 */

    /* Keep generating primes until we have enough matching ones. */
    while (match_cnt < n) {
        idx_t new_limit = current_limit ? current_limit * 2 : 100ULL;
        if (new_limit <= current_limit) new_limit = current_limit + 100ULL;
        generate_primes_up_to(new_limit);
    }

    return (seq_t)match_list[n - 1];
}
