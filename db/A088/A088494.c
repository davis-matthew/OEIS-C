/*
 * A088494  Let P(n,k) = n!/(Product_{i=1..pi(n)/2^(k-1)} prime(i)) be an
 *          integer matrix of "partial" factorials. Then a(n) = sum_{k=1..8}
 *          floor( P(n,k)/P(n-1,k)).
 * https://oeis.org/A088494
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..8} ⌊P(n,k)/P(n-1,k)⌋ where P(n,k) = n! /
 *            ∏_{i=1..⌊π(n)/2^(k-1)⌋} prime(i) (π = prime counting)
 * Range:     u8 n <= 31, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes up to the largest n seen so far */
static uint64_t *primes = NULL;
static size_t    prime_cnt = 0;
static idx_t     prime_lim = 0;   /* maximum n for which primes are cached */

/* Ensure that primes[0..prime_cnt-1] contains all primes ≤ n */
static void ensure_primes_upto(idx_t n)
{
    if (n <= prime_lim) return;
    /* Allocate new array of primes up to n */
    uint64_t *new_primes = NULL;
    size_t   new_cnt = 0;
    free(primes);    /* discard old array */
    build_primes_upto(n, &new_primes, &new_cnt);
    primes = new_primes;
    prime_cnt = new_cnt;
    prime_lim = n;
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* offset is 2 */

    /* Ensure prime array is up to date */
    ensure_primes_upto(n);

    /* Prime count up to n */
    size_t pi_n = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (primes[i] <= (uint64_t)n) pi_n = i + 1;
        else break;
    }

    bool n_is_prime = (pi_n > 0 && primes[pi_n - 1] == (uint64_t)n);

    seq_t total = 0;
    for (int k = 1; k <= 8; ++k) {
        /* power = 2^(k-1) */
        uint64_t pow = 1ULL << (k - 1);
        seq_t ratio = (seq_t)n;  /* default value when threshold does not increase */

        if (n_is_prime && (pi_n % pow) == 0) {
            /* Threshold increased: divide by the prime at index pi_n/pow (1‑based) */
            size_t idx = (pi_n >> (k - 1)) - 1;   /* zero‑based index */
            ratio = ratio / (seq_t)primes[idx];
        }
        total += ratio;
    }
    return total;
}
