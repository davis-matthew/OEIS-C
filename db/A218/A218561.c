/*
 * A218561  4-gap primes: Prime p is a term iff there is no prime between 4*p
 *          and 4*nextprime(p), where nextprime=A151800.
 * https://oeis.org/A218561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 668, u32 n <= 4911, u64 n <= 4911,
 *            u128 n <= 4911, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 4‑gap primes: a(n) is the n‑th prime p such that no
 * prime lies strictly between 4*p and 4*nextprime(p). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Keep a cache of the sequence values seen so far. */
static seq_t *seq_terms = NULL;
static size_t seq_cap   = 0;
static size_t seq_cnt   = 0;          /* number of terms stored */

/* The next prime to test in the overall prime list.  Initially 2. */
static uint64_t next_test_prime = 2;

/* Grow the cache array if necessary. */
static void grow_terms(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_terms = (seq_t *)realloc(seq_terms, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Find the smallest prime > start.  Uses the is_prime helper. */
static inline uint64_t next_prime_internal(uint64_t start)
{
    if (start < 2) return 2;
    uint64_t p = start + 1 + (start % 2);   /* first odd > start */
    for (;; p += 2) {
        if (is_prime(p)) return p;
    }
}

/* Main sequence function.  The result is returned as a seq_t value. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* defined for 1‑based indexing */

    /* Generate terms until we have at least n of them. */
    while (seq_cnt < (size_t)n) {
        /* Find next prime after current_test_prime. */
        uint64_t current = next_test_prime;
        uint64_t nxt     = next_prime_internal(current);

        /* Is there any prime in the open interval (4*current, 4*nxt)? */
        uint64_t interval_start = 4ull * current + 1;
        uint64_t interval_end   = 4ull * nxt - 1;
        uint64_t q = next_prime_internal(interval_start - 1); /* first prime >= interval_start */
        bool has_gap_prime = (q > interval_end);              /* no prime inside interval */

        if (has_gap_prime) {
            grow_terms(seq_cnt + 1);
            seq_terms[seq_cnt++] = (seq_t)current;
        }

        /* Move to the next prime in the global prime list. */
        next_test_prime = nxt;
    }

    /* Return the n‑th term (array is 0‑based). */
    return seq_terms[(size_t)n - 1];
}
