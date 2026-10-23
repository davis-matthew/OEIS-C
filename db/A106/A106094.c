/*
 * A106094  Primes with maximal digit = 8.
 * https://oeis.org/A106094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has8_max8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by iterating over integers, testing
   primality with a small prime list, and selecting those
   containing at least one digit 8 and no digit > 8. */

/* No external headers beyond the build system's types are required. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has8_max8.h"

#define MAX_PRIMES   65536   /* enough for n up to many thousands          */
#define MAX_TERMS    65536   /* enough for the first many terms of A106094 */

/* Static tables and indices for caching. */
static uint64_t prime_tbl[MAX_PRIMES];
static idx_t   prime_cnt = 0;

static uint64_t term_tbl[MAX_TERMS];
static idx_t   term_cnt  = 0;

static uint64_t next_candidate = 2;

/* Quick primality test using the prime table. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* 2 is the only even prime. */
    if (n == 2) return true;
    if ((n & 1ULL) == 0) return false;

    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is 1‑based */

    /* Generate terms up to the required index, reusing cached results. */
    while (term_cnt < n) {
        uint64_t cand = next_candidate;

        /* Advance candidate for next iteration. */
        if (cand == 2) {
            next_candidate = 3;
        } else {
            next_candidate = cand + 2;
        }

        /* Test primality, storing the prime if successful. */
        bool prime = false;
        if (cand < 2) {
            prime = false;
        } else {
            /* Expand prime table if necessary. */
            if (prime_cnt < MAX_PRIMES) {
                prime = is_prime_uint64(cand);
                if (prime) {
                    prime_tbl[prime_cnt++] = cand;
                }
            } else {
                /* If we run out of space, fallback to direct test
                   but do not store the prime to avoid corruption. */
                prime = is_prime_uint64(cand);
            }
        }

        if (prime && has8_max8(cand)) {
            /* Store term if space permits. */
            if (term_cnt < MAX_TERMS) {
                term_tbl[term_cnt++] = cand;
            }
        }
    }

    /* Return the requested term, casting to seq_t. */
    return (seq_t)term_tbl[n - 1];
}
