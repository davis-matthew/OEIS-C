/*
 * A089144  Primes p such that 6*p-5 and 6*p-1 are primes.
 * https://oeis.org/A089144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 372, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 6*p-5 and 6*p-1 are primes (OEIS A089144) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Static cache of computed terms */
static seq_t *seq_cache  = NULL;   /* array of computed terms          */
static size_t seq_len   = 0;       /* number of terms stored           */
static size_t seq_cap   = 0;       /* current capacity of seq_cache    */
static idx_t  last_candidate = 1;  /* last integer examined (prime)   */

/* Ensure the cache contains at least n terms (n >= 1). */
static void ensure_n(idx_t n)
{
    if (n == 0) return; /* a(0) is undefined */

    /* Initial allocation if needed */
    if (seq_cache == NULL) {
        seq_cap   = 1024;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (seq_cache == NULL) exit(EXIT_FAILURE);
    }

    /* Grow the cache if necessary */
    if (seq_len + (n - seq_len) > seq_cap) {
        size_t new_cap = seq_cap * 2;
        while (new_cap < seq_len + (n - seq_len)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cap   = new_cap;
    }

    /* Generate new terms until we reach n */
    while (seq_len < n) {
        /* Next candidate integer */
        idx_t p = ++last_candidate;

        /* Only consider prime candidates */
        if (!isPrimeGeneric((seq_t)p)) continue;

        /* Compute 6*p-5 and 6*p-1 */
        seq_t sp = (seq_t)p;
        seq_t t1 = sp * 6 - 5; /* 6*p - 5 */
        seq_t t2 = sp * 6 - 1; /* 6*p - 1 */

        /* Check the condition */
        if (isPrimeGeneric(t1) && isPrimeGeneric(t2)) {
            seq_cache[seq_len++] = sp;
        }
    }
}

/* Public function returning the n-th term (a(1)=first term) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* Undefined for n=0, but we return 0 */
    ensure_n(n);
    return seq_cache[n - 1];
}
