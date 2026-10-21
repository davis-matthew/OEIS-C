/*
 * A065305  Triangular array giving means of two odd primes: T(n,k) = (n-th
 *          prime + k-th prime)/2, n >= k >= 2.
 * https://oeis.org/A065305
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (p_i + p_j)/2 where i and j satisfy (i‑1)*i/2 < n‑1 ≤
 *            i*(i+1)/2 and j = (n‑1) – (i‑1)*i/2. p_k is the k‑th odd prime
 *            (p_1 = 3). The primes are generated on demand by a simple
 *            incremental sieve
 * Range:     u8 n <= 1484, u16 n <= 7876, u32 n <= 7876, u64 n <= 7876,
 *            u128 n <= 7876, big any n
 * Helpers:   row_from_m
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/row_from_m.h"

/* static storage for generated odd primes, indexed from 0, where
   primes[0] == 3 is the 1st odd prime.                             */
static uint64_t *primes      = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_cap    = 0;

/* Extend the prime table until it contains at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* Allocate / grow the table. */
    if (prime_cap == 0) {
        prime_cap = 64;
        primes    = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cnt >= prime_cap) {
        prime_cap <<= 1;
        primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
    }

    /* Start generating primes from the last known prime + 2 (odd).   */
    uint64_t candidate = (prime_cnt == 0) ? 3 : primes[prime_cnt - 1] + 2;

    while (prime_cnt < need) {
        bool is_prime = true;
        /* Check divisibility by known primes up to sqrt(candidate).     */
        for (size_t idx = 0; idx < prime_cnt; ++idx) {
            uint64_t p = primes[idx];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[prime_cnt++] = candidate;
            if (prime_cnt >= prime_cap) {
                prime_cap <<= 1;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
        }
        candidate += 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* undefined for n<2, but harmless */

    /* m = index in the triangular array starting from 1. */
    idx_t m = n - 1u;

    /* Determine row 'i' and column 'j'. */
    idx_t i = row_from_m(m);
    idx_t prev_rows = (i - 1u) * i / 2;   /* elements before row i */
    idx_t j = m - prev_rows;              /* 1-based column */

    /* Ensure we have enough primes. */
    ensure_primes(i > j ? i : j);

    /* p_i and p_j are the i‑th and j‑th odd primes.              */
    uint64_t pi = primes[i - 1];
    uint64_t pj = primes[j - 1];

    seq_t res = ((seq_t)pi + (seq_t)pj) / 2;
    return res;
}
