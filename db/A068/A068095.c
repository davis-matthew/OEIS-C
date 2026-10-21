/*
 * A068095  All primes dividing each term are Fibonacci numbers.
 * https://oeis.org/A068095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068095: numbers all of whose prime factors are Fibonacci primes. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_u64.h"

/* Fibonacci primes: first few primes that are Fibonacci numbers */
static const uint64_t fib_primes[] = {
    2ULL, 3ULL, 5ULL, 13ULL, 89ULL, 233ULL,
    1597ULL, 28657ULL, 514229ULL, 433494437ULL
};
static const size_t fib_prime_cnt = sizeof(fib_primes)/sizeof(fib_primes[0]);

/* Humble number cache */
static seq_t *humble = NULL;      /* humble[0] = 1 */
static size_t humble_size = 0;    /* number of terms stored */
static size_t humble_cap  = 0;    /* allocated capacity */

/* Indexes for each prime: humble[idx[i]] is the current multiplier for fib_primes[i] */
static size_t *prime_idx = NULL;
static size_t prime_cnt  = 0;

/* Ensure term cache has at least n terms (1‑based). */
static void ensure_terms(idx_t n)
{
    if (humble_size == 0) {
        /* first run: initialise cache */
        humble_cap = 32;
        humble = (seq_t*)malloc(humble_cap * sizeof(seq_t));
        humble[0] = (seq_t)1;
        humble_size = 1;

        prime_cnt = fib_prime_cnt;
        prime_idx = (size_t*)malloc(prime_cnt * sizeof(size_t));
        for (size_t i = 0; i < prime_cnt; ++i) prime_idx[i] = 0;
    }

    while (humble_size < (size_t)n) {
        seq_t best = 0;
        bool first = true;

        /* find the smallest candidate */
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t cand = mul_u64(fib_primes[i], humble[prime_idx[i]]);
            if (first || cand < best) {
                best = cand;
                first = false;
            }
        }

        /* append the next term */
        if (humble_size == humble_cap) {
            humble_cap = humble_cap ? humble_cap << 1 : 32;
            humble = (seq_t*)realloc(humble, humble_cap * sizeof(seq_t));
        }
        humble[humble_size++] = best;

        /* advance indices that produced the chosen value */
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t cand = mul_u64(fib_primes[i], humble[prime_idx[i]]);
            if (cand == best) ++prime_idx[i];
        }
    }
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, not used in OEIS offset */
    ensure_terms(n);
    return humble[(size_t)n - 1];
}
