/*
 * A129257  Primes p such that k-1, k+1 are composite, where k = absolute
 *          value of q^2 - p*r and p, q, r are consecutive primes.
 * https://oeis.org/A129257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that for consecutive primes * p, q,
 *            r: k = |q² - p·r|, with k ≥ 3, and both k‑1 and k+1 composite. *
 *            The sequence is 1‑based; a(1) = 53
 * Range:     u8 none, u16 n <= 14, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

/* Dynamic storage for the sequence values. */
static seq_t *seq_vals   = NULL;
static size_t seq_cap    = 0; /* current allocated capacity */
static size_t seq_cnt    = 0; /* number of terms stored */

/* Dynamic storage for the prime list. */
static uint64_t *prime_list = NULL;
static size_t    primes_cap = 0;
static size_t    primes_cnt = 0;

/* Index of the first prime of the next triple to examine. */
static idx_t triple_idx = 0;

/* ------------------------------------------------------------------ */
static void grow_seq(size_t needed)
{
    if (needed <= seq_cap) return;
    seq_cap = needed > seq_cap ? needed * 2 : 2;
    seq_vals = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
static void grow_primes(size_t needed)
{
    if (needed <= primes_cap) return;
    primes_cap = needed > primes_cap ? needed * 2 : 2;
    prime_list = (uint64_t *)realloc(prime_list, primes_cap * sizeof(uint64_t));
}

/* ------------------------------------------------------------------ */
/* Generate sequence values until at least `target` terms are found. */
static void generate_until(idx_t target)
{
    while (seq_cnt < target)
    {
        /* Ensure we have at least three primes for the current triple. */
        while (primes_cnt < triple_idx + 3)
        {
            uint64_t start = (primes_cnt == 0 ? 2
                              : prime_list[primes_cnt - 1] + 1);
            uint64_t nextp = next_prime(start);
            grow_primes(primes_cnt + 1);
            prime_list[primes_cnt++] = nextp;
        }

        /* Current triple of consecutive primes. */
        uint64_t p = prime_list[triple_idx];
        uint64_t q = prime_list[triple_idx + 1];
        uint64_t r = prime_list[triple_idx + 2];

        /* k = |q² – p·r| computed in seq_t to avoid overflow. */
        seq_t qsq = (seq_t)q * (seq_t)q;
        seq_t prod = (seq_t)p * (seq_t)r;
        seq_t k = (qsq >= prod) ? (qsq - prod) : (prod - qsq);

        if (k >= (seq_t)3)
        {
            uint64_t km1 = (uint64_t)(k - 1);
            uint64_t kp1 = (uint64_t)(k + 1);

            if (!is_prime(km1) && !is_prime(kp1))
            {
                grow_seq(seq_cnt + 1);
                seq_vals[seq_cnt++] = (seq_t)p;
            }
        }

        ++triple_idx;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    if (seq_cnt < n)
        generate_until(n);

    return seq_vals[n - 1];
}
