/*
 * A102478  Numbers n such that the concatenations (2*n),(2*n-1) and
 *          (2*n),(2*n+1) give twin primes.
 * https://oeis.org/A102478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 396, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   digits, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A102478: numbers n such that the decimal concatenations
   (2*n , 2*n-1) and (2*n , 2*n+1) form a pair of twin primes.  The
   function returns the n-th such number (1‑based).  It uses a memoised
   sequential search and a deterministic Miller‑Rabin primality test
   for 64‑bit integers via is_prime(uint64_t). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

static seq_t *cache = NULL;      /* stored terms of the sequence   */
static idx_t cache_size = 0;     /* number of terms already stored */
static idx_t next_candidate = 1; /* next n to test */

/* Helper: test whether n satisfies the twin‑prime concatenation
   property.  All arithmetic is performed in 64‑bit integers, which
   is sufficient for the first ~10^6 candidates. */
static bool good_candidate(idx_t cand) {
    uint64_t two = 2ull * cand;                    /* 2*n          */
    int d1 = digits((idx_t)(two - 1));              /* digits of 2*n-1 */
    uint64_t pow10d1 = (uint64_t)pow10(d1);          /* 10^d1        */
    uint64_t p1 = two * pow10d1 + (two - 1);        /* concat 2n(2n-1) */
    if (!is_prime(p1)) return false;

    int d2 = digits((idx_t)(two + 1));              /* digits of 2*n+1 */
    uint64_t pow10d2 = (uint64_t)pow10(d2);
    uint64_t p2 = two * pow10d2 + (two + 1);        /* concat 2n(2n+1) */
    if (!is_prime(p2)) return false;

    return true; /* both are prime, hence twins */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for zero index */
    /* extend the cache until it contains the requested term */
    while (cache_size < n) {
        if (good_candidate(next_candidate)) {
            /* grow the array by one element */
            seq_t *tmp = (seq_t*)realloc(cache,
                                         sizeof(seq_t) * (cache_size + 1));
            if (!tmp) return 0;     /* allocation failed – abort */
            cache = tmp;
            cache[cache_size] = (seq_t)next_candidate; /* store the n */
            cache_size++;
        }
        next_candidate++;            /* next integer to test */
    }
    return cache[n - 1];
}
