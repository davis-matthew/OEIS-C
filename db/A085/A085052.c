/*
 * A085052  Start with n and continue adding primes until one reaches a prime
 *          > n. Or, sequence gives n + P(k) where P(k)=sum_{k=1..n} prime(k)
 *          and k is the least value such that n + P(k) is prime and > n.
 * https://oeis.org/A085052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A085052: a(n) = n + sum_{i=1..k} prime(i) with the first
   k such that the sum is a prime > n.  */
/* Sequential primes and cumulative sums are generated on demand. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/is_prime_small.h"   /* is_prime_small(idx_t) */

/* Static tables of primes and cumulative sums. */
static uint64_t *primes = NULL;   /* primes[0] = 2, primes[1] = 3, ... */
static size_t    primes_cnt = 0;  /* number of primes stored          */
static seq_t   *cum = NULL;       /* cum[i] = sum of first i primes   */
static idx_t    prime_limit = 0;   /* upper bound that has been sieved  */

/* ------------------------------------------------------------------ */
/* Allocate fibron for a given upper bound 'limit' (≥ 2).               */
/* The function may be called multiple times, increasing 'limit'.      */
/* ------------------------------------------------------------------ */
static void
ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)                /* already enough data */
        return;

    /* O(N log log N) sieve of Eratosthenes (limit is small, < ~1e6) */
    size_t sz = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(sz, sizeof(bool));
    if (!is_comp) abort();

    size_t root = (size_t)sqrt((double)limit) + 1;
    for (size_t i = 2; i <= root; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes and build new arrays */
    size_t new_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (!is_comp[i]) ++new_cnt;

    uint64_t *new_pr = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
    seq_t   *new_cum = (seq_t   *)malloc((new_cnt + 1) * sizeof(seq_t));
    if (!new_pr || !new_cum) abort();

    size_t idx = 0;
    new_cum[0] = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!is_comp[i]) {
            new_pr[idx] = (uint64_t)i;
            new_cum[idx + 1] = new_cum[idx] + (seq_t)i;
            ++idx;
        }
    }

    free(is_comp);
    free(primes);
    free(cum);

    primes = new_pr;
    primes_cnt = new_cnt;
    cum = new_cum;
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Return a(n) for n >= 1.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static bool first_call = true;
    if (first_call) {
        first_call = false;
        ensure_primes_up_to(200000);          /* initial limit */
    }

    for (;;) {
        /* Scan through primes until a suitable prime is found. */
        for (size_t k = 0; k < primes_cnt; ++k) {
            seq_t candidate = (seq_t)n + cum[k + 1];
            if (candidate > (seq_t)n && is_prime_small((idx_t)candidate))
                return candidate;
        }

        /* No candidate found – double the sieve limit. */
        idx_t new_limit = prime_limit * 2;
        if (new_limit == 0) new_limit = 100000;
        ensure_primes_up_to(new_limit);
    }
}
