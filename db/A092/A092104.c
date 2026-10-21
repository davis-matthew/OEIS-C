/*
 * A092104  Primes of form p*q + 4, with prime p and q.
 * https://oeis.org/A092104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092104: Primes of the form p*q + 4, where p and q are primes. */
/* We generate primes up to a fixed limit and test each prime to see whether
   its value minus 4 is a product of exactly two primes (allowing the powers
   p = q).  The sequence is stored in a dynamic array so arbitrary calls
   to a(n) with n up to about 10 000 are answered in O(n) time. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes.h"

static const idx_t PRIME_LIMIT = 3000000;          /* sieve limit */
static idx_t *primes = NULL;                      /* prime list */
static size_t primeCnt = 0;                       /* number of primes */

/* Dynamic array for sequence values */
static seq_t *seqArr = NULL;
static size_t seqLen = 0;                         /* number of generated terms */
static size_t seqCap = 0;                         /* allocated capacity */

static size_t primePos = 0;                      /* next prime to examine */

/* Initialise primes and sequence storage once */
static void initialise(void) {
    if (primes != NULL) return;                  /* already initialised */

    build_primes(PRIME_LIMIT, &primes, &primeCnt);

    seqCap = 128;
    seqArr = (seq_t *)malloc(seqCap * sizeof(seq_t));
    seqLen = 0;
    primePos = 0;
}

/* Append a value to the sequence dynamic array */
static void append_seq(seq_t val) {
    if (seqLen == seqCap) {
        seqCap <<= 1;
        seqArr = (seq_t *)realloc(seqArr, seqCap * sizeof(seq_t));
    }
    seqArr[seqLen++] = val;
}

/* Test whether n-4 is a product of exactly two primes (p*q).   */
static bool semiprime_pq_plus4(idx_t prime_val) {
    idx_t v = prime_val - 4;
    if (v < 4) return false;           /* too small to be product of two primes */

    /* Find smallest prime factor f of v */
    for (size_t i = 0; i < primeCnt && (idx_t)primes[i] * (idx_t)primes[i] <= v; ++i) {
        idx_t f = primes[i];
        if (v % f == 0) {
            idx_t g = v / f;
            if (g > 1 && is_prime(g)) return true;   /* p*q found, equality allowed */
        }
    }
    return false;
}

/* Ensure that we have generated the sequence up to at least nth term. */
static void ensure_n(idx_t n) {
    if (n <= seqLen) return;            /* already have it */

    while (seqLen < n && primePos < primeCnt) {
        idx_t p_val = primes[primePos++];
        if (p_val < 13) continue;        /* sequence starts at 13 */
        if (semiprime_pq_plus4(p_val)) {
            append_seq(p_val);
        }
    }
    /* Note: With PRIME_LIMIT set to 3 000 000, we will have more than 10 000
       terms, so primePos should never reach primeCnt for n up to 10 000. */
}

/* External function: a(n) */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined for n == 0; return 0 as a safety value */
        return (seq_t)0;
    }
    initialise();
    ensure_n(n);
    return seqArr[n - 1];
}
