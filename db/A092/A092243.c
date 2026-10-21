/*
 * A092243  Score at stage n in "tug of war" between prime gap increases vs.
 *          prime gap decreases: start with score = 0 at n = 1 and at stage n
 *          = k > 1, increase (resp. decrease) the score by 1 if the k-th
 *          prime gap is greater (resp. less) than the previous prime gap.
 * https://oeis.org/A092243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9661, i16 n <= 9661, i32 n <= 9661, i64 n <= 9661,
 *            i128 n <= 9661, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the tug‑of‑war score from prime gaps.
   a(1) = 0.
   For n > 1, a(n) = a(n‑1) + sign(gap(n) – gap(n‑1)),
   where gap(k) = prime(k+1) – prime(k). */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"   /* returns uint32_t get_prime(idx_t k); */

static idx_t *prime_cache      = NULL;   /* primes[0] = 2, primes[1] = 3 ... */
static size_t prime_cache_cap  = 0;
static size_t prime_cache_len  = 0;

static seq_t *score_cache      = NULL;   /* score_cache[i] = a(i+1)   */
static size_t score_cache_cap  = 0;
static size_t score_cache_len  = 0;

static seq_t last_gap = 0;      /* last computed prime gap */

/* Ensure prime_cache contains the first (n+1) primes.  */
static void ensure_primes(size_t n_plus_one)
{
    if (prime_cache_len >= n_plus_one)
        return;

    if (prime_cache_cap < n_plus_one) {
        size_t new_cap = n_plus_one * 2;
        idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        prime_cache      = tmp;
        prime_cache_cap  = new_cap;
    }

    for (size_t i = prime_cache_len; i < n_plus_one; ++i) {
        /* get_prime is 1‑based; i is 0‑based. */
        prime_cache[i] = (idx_t)get_prime((idx_t)(i + 1));
    }
    prime_cache_len = n_plus_one;
}

/* Ensure score_cache contains the scores for indices 1 … n.  */
static void ensure_scores(size_t n)
{
    if (score_cache_len >= n)
        return;

    if (score_cache_cap < n) {
        size_t new_cap = n * 2;
        seq_t *tmp = (seq_t *)realloc(score_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        score_cache      = tmp;
        score_cache_cap  = new_cap;
    }

    /* Compute scores from score_cache_len to n.  */
    if (score_cache_len == 0) {
        /* First term: a(1) = 0.  */
        score_cache[0] = (seq_t)0;
        /* First prime gap for next comparison.  */
        last_gap = (seq_t)(prime_cache[1] - prime_cache[0]);
        score_cache_len = 1;
    }

    for (size_t i = score_cache_len + 1; i <= n; ++i) {    /* i is 1‑based */
        seq_t current_gap = (seq_t)(prime_cache[i] - prime_cache[i - 1]);
        seq_t inc = 0;
        if (current_gap > last_gap)
            inc = (seq_t)1;
        else if (current_gap < last_gap)
            inc = (seq_t)(-1);
        /* score_cache[i-1] = a(i);  a(i) = a(i-1) + inc. */
        score_cache[i - 1] = score_cache[i - 2] + inc;
        last_gap = current_gap;
    }
    score_cache_len = n;
}

/* Public entry point for the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined case, return 0 for safety */

    /* We need primes up to index n+1 (gap uses prime(n+1)).  */
    ensure_primes((size_t)n + 1);
    /* Compute scores up to n.  */
    ensure_scores((size_t)n);

    return score_cache[(size_t)(n - 1)];
}
