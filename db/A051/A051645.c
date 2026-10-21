/*
 * A051645  Primes p such that 30*p+1 is also prime.
 * https://oeis.org/A051645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1200, u32 n <= 55, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Sequence A051645: primes p such that 30*p + 1 is also prime.
   The sequence is 1‑based: a(1) = 2. */

static seq_t *cache      = NULL;   /* storage for discovered terms   */
static size_t cache_cap   = 0;      /* current capacity of cache      */
static idx_t  cache_len   = 0;      /* number of terms stored         */
static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Append a new valid term to the cache, resizing if necessary. */
static void add_candidate(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined for n < 1; safe default */

    uint64_t p = next_candidate;        /* start scanning from where we left off */

    while (cache_len < n) {             /* generate terms until we have n  */
        if (is_prime(p)) {              /* only consider prime candidates   */
            seq_t q = (seq_t)p * 30 + 1;
            if (is_prime_big(q)) {      /* 30*p+1 must also be prime         */
                add_candidate((seq_t)p);/* store the term (p)                */
            }
        }
        ++p;
    }

    next_candidate = p;                 /* remember upper bound for next call */

    return cache[n - 1]; /* cache is 0‑based, n is 1‑based */
}
