/*
 * A090180  Primes p = prime(n) such that p + sum-of-digits(p) +- 1 =
 *          prime(n+1).
 * https://oeis.org/A090180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 975 (2026-11-01)
 * Helpers:   digit_sum_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090180: a(n) = the n‑th prime p such that p + sum_of_digits(p) ± 1
   equals the next prime.  a(1)=2. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum_idx_to_seq.h"

/* Simple trial division primality test using the primes discovered so far.
   Since primes are added in increasing order, the list contains all
   primes ≤ sqrt(candidate), so the test is correct.                 */
static inline bool is_prime_trial(idx_t candidate,
                                  idx_t const *primes, size_t primes_len)
{
    if (candidate < 2)
        return false;
    for (size_t i = 0; i < primes_len; ++i) {
        idx_t p = primes[i];
        if (p > candidate / p)  /* p > sqrt(candidate)  */
            break;
        if (candidate % p == 0)
            return false;
    }
    return true;
}

/* Static storage for the sequence values and the primes used in the test.
   The limits are chosen to comfortably cover the first 10 000 terms.
   The arrays are statically allocated so the code compiles in both C
   and C++.  The compiler will automatically default‑construct objects
   of type seq_t when building as C++.  */
#define MAX_PRIMES 500000
#define MAX_SEQ    200000

static seq_t  seq_values[MAX_SEQ];
static size_t seq_len = 0;

static idx_t  primes[MAX_PRIMES];
static size_t primes_len = 0;

/* Main function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0)            /* invalid offset – safeguard */
        return 0;

    /* Generate primes and check the property until we have n terms. */
    while (seq_len < n)
    {
        idx_t cand;

        /* Find the next prime after the last discovered one. */
        if (primes_len == 0) {
            cand = 2;
        } else {
            cand = primes[primes_len - 1] + 1;
            if ((cand & 1) == 0)      /* make odd */
                ++cand;
            while (!is_prime_trial(cand, primes, primes_len))
                cand += 2;
        }

        /* If we have a previous prime, test the defining property. */
        if (primes_len >= 1)
        {
            idx_t prev = primes[primes_len - 1];
            seq_t sumd = digit_sum_idx_to_seq(prev);
            seq_t plus  = (seq_t)prev + sumd;
            seq_t cand_seq = (seq_t)cand;
            if (cand_seq == plus + (seq_t)1 || cand_seq == plus - (seq_t)1)
                seq_values[seq_len++] = (seq_t)prev;
        }

        /* Store the new prime for subsequent iterations. */
        primes[primes_len++] = cand;
    }

    return seq_values[n - 1];
}
