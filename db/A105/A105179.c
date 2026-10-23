/*
 * A105179  Numbers having in decimal representation the same final digit as
 *          their largest proper divisor has; a(1) = 1.
 * https://oeis.org/A105179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th positive integer m (m≥1) such that the units
 * digit of m equals the units digit of its largest proper divisor.
 * For m>1 the largest proper divisor is m / spf(m), where spf(m) is the
 * smallest prime factor of m.  For a prime m the divisor is 1. 
 * The function generates terms incrementally and reuses previously found
 * values; it works for any seq_t (plain or big‑int) and any idx_t
 * (here uint64_t). 
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* static storage for the sequence */
static seq_t *terms     = NULL;
static size_t terms_cap = 0;
static size_t terms_sz  = 0;

/* static storage for the list of primes up to sqrt(current_m) */
static uint64_t *primes   = NULL;
static size_t    primes_cap = 0;
static size_t    primes_cnt = 0;

/* next integer to test (starting at 2, since 1 is handled separately) */
static uint64_t current_m = 2;

seq_t a(idx_t n)
{
    /* initialise static state on first call */
    if (terms_cap == 0) {
        terms_cap = 64;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms[0] = (seq_t)1;          /* a(1) = 1 */
        terms_sz = 1;

        primes_cap = 64;
        primes     = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
    }

    /* generate terms until we have at least n of them */
    while (terms_sz < n) {
        uint64_t m  = current_m;
        uint64_t spf = m;

        /* find smallest prime factor using sieved primes list */
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= m; ++i) {
            if (m % primes[i] == 0) {
                spf = primes[i];
                break;
            }
        }

        /* if m itself is prime, record it */
        if (spf == m) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap * 2 + 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = m;
        }

        /* test the units‑digit condition */
        uint64_t d = m / spf;          /* largest proper divisor */
        if ((m % 10) == (d % 10)) {
            if (terms_sz == terms_cap) {
                terms_cap = terms_cap * 2 + 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[terms_sz++] = (seq_t)m;
        }

        ++current_m;
    }

    return terms[n-1];
}
