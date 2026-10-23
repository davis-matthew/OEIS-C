/*
 * A120330  Primes not congruent to +- 1, 3, or 4 (mod 13).
 * https://oeis.org/A120330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   ensure_cap_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th term of OEIS A120330:
   primes that are not congruent to ±1, ±3, or ±4 (mod 13).
   It handles calls in arbitrary order and memoizes previously
   found terms. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_cap_u64.h"

static seq_t *seq = NULL;          /* cache of sequence terms          */
static size_t seq_len = 0;         /* number of stored terms           */
static size_t seq_cap = 0;         /* capacity of seq buffer           */

static uint64_t *primes = NULL;    /* list of known primes             */
static size_t primes_len = 0;      /* number of known primes           */
static size_t primes_cap = 0;      /* capacity of primes buffer        */
static uint64_t last_candidate = 1;/* last integer checked for prime   */

/* Resize a buffer if necessary. Realloc is used for simplicity. */
static void ensure_cap(seq_t **buf, size_t *len, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 128 : *cap * 2;
        while (newcap < need) newcap *= 2;
        *buf = (seq_t*)realloc(*buf, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Simple primality test using the known primes list.
   Assumes that all primes <= sqrt(num) are already in primes[]. */
static bool is_prime_candidate(uint64_t num)
{
    if (num < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > num) break;
        if (num % p == 0) return false;
    }
    return true;
}

/* Generate primes up to satisfy at least target terms in seq. */
static void generate_more(size_t target)
{
    while (seq_len < target) {
        /* find next prime */
        last_candidate++;
        while (!is_prime_candidate(last_candidate)) last_candidate++;

        /* store prime in list */
        ensure_cap_u64(&primes, &primes_len, &primes_cap, primes_len + 1);
        primes[primes_len++] = last_candidate;

        /* check congruence condition */
        if (last_candidate == 13) {
            ensure_cap(&seq, &seq_len, &seq_cap, seq_len + 1);
            seq[seq_len++] = (seq_t)last_candidate;
            continue;
        }
        uint64_t r = last_candidate % 13u;
        if (r == 1 || r == 3 || r == 4 || r == 9 || r == 10 || r == 12)
            continue;   /* excluded residue classes */
        ensure_cap(&seq, &seq_len, &seq_cap, seq_len + 1);
        seq[seq_len++] = (seq_t)last_candidate;
    }
}

seq_t a(idx_t n)
{
    /* n is 1-based; return the n-th term. */
    if (n <= 0) return (seq_t)0;  /* undefined for non‑positive indices */
    /* generate terms until we have at least n of them */
    generate_more((size_t)n);
    return seq[(size_t)n - 1];
}
