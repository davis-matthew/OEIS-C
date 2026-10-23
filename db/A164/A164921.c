/*
 * A164921  a(1)=0, a(2)=1. For n >=3, a(n) = the smallest integer > a(n-1)
 *          that is coprime to every sum of any two distinct earlier terms of
 *          this sequence.
 * https://oeis.org/A164921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A164921
   a(1)=0, a(2)=1.
   For n≥3, a(n) is the smallest integer >a(n‑1)
   that is coprime to every sum of any two distinct earlier terms.

   The implementation builds the sequence on demand.
   It keeps an array of the terms and a boolean array
   marking the prime divisors of all pairwise sums seen so far.
   A candidate value is accepted when none of its prime factors
   appear in that set.  All prime factors of a sum are determined
   with a pre‑computed smallest‑prime‑factor table (SPF). */

#include <stdlib.h>
#include <stdbool.h>

/* SPF table limits */
#define SPF_LIMIT 500000U

static seq_t *cache = NULL;          /* computed terms */
static idx_t cache_len = 0;          /* number of terms computed */

static bool *bad_prime = NULL;      /* good[i] == true   if prime i divides any sum */
static uint32_t *spf = NULL;        /* smallest prime factor table */

/* initialise SPF and bad_prime tables */
static void init_sieve(void)
{
    if (spf != NULL) return;  /* already initialised */

    spf = (uint32_t *)malloc((SPF_LIMIT + 1) * sizeof(uint32_t));
    for (size_t i = 0; i <= SPF_LIMIT; ++i) spf[i] = (uint32_t)i;
    spf[0] = 1; spf[1] = 1;

    for (size_t i = 2; i * i <= SPF_LIMIT; ++i)
        if (spf[i] == (uint32_t)i)
            for (size_t j = i * i; j <= SPF_LIMIT; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;

    bad_prime = (bool *)calloc(SPF_LIMIT + 1, sizeof(bool));
}

/* factorise x and mark its prime divisors in bad_prime */
static void mark_primes_of_pairwise_sum(seq_t x)
{
    size_t v = (size_t)x;
    while (v > 1) {
        uint32_t p = spf[v];
        if (p == 0 || p > SPF_LIMIT) {
            /* if spf out of bounds we treat v itself as a prime */
            bad_prime[(size_t)p] = true;
            break;
        }
        bad_prime[p] = true;
        while (v % p == 0) v /= p;
    }
}

/* add all sums formed with new_term and all earlier terms */
static void update_sums_with_new_term(seq_t new_term, idx_t new_idx)
{
    for (idx_t i = 0; i < new_idx; ++i) {
        seq_t sum = new_term + cache[i];
        mark_primes_of_pairwise_sum(sum);
    }
}

/* test whether cand is coprime with all known sums */
static bool candidate_is_valid(seq_t cand)
{
    size_t v = (size_t)cand;
    while (v > 1) {
        uint32_t p = spf[v];
        if (p == 0 || p > SPF_LIMIT) {
            /* v itself is a prime larger than SPF_LIMIT;
               but no sum exceeds SPF_LIMIT, so it cannot be bad. */
            break;
        }
        if (bad_prime[p]) return false;
        while (v % p == 0) v /= p;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used, but safe */

    init_sieve();

    if (cache_len < 2) {
        /* initialise with a(1)=0, a(2)=1 */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow cache to contain term n */
    while (cache_len < n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + 1;

        /* search for smallest valid candidate */
        while (!candidate_is_valid(cand)) ++cand;

        /* append the new term */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = cand;
        /* update sums with the new term */
        update_sums_with_new_term(cand, cache_len);
        ++cache_len;
    }

    return cache[n - 1];
}
