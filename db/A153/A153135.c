/*
 * A153135  Primes p such that 6*p - 7 is also prime.
 * https://oeis.org/A153135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 447, u32 n <= 276, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = prime p such that 6*p – 7 is also prime. */

/* The routine builds the sequence on demand, caching results for
   subsequent calls.  It tests each integer candidate for primality
   with the provided is_prime_big routine and accepts the prime if
   the companion value 6*p‑7 is also prime.  The cache grows as
   necessary, so the function works for any order of calls and for
   large n.  All arithmetic is performed in seq_t; the candidate
   counter is held in seq_t and advanced only by the necessary
   increments. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset: queries start at 1 */

    /* ---------- Cache of computed terms ---------- */
    static seq_t *cache = NULL;          /* array of accepted primes   */
    static size_t cache_cap = 0;          /* allocated size               */
    static size_t cache_len = 0;          /* number of terms stored      */

    /* ---------- Candidate counter for prime search ---------- */
    static seq_t candidate = 0;           /* next integer to test       */
    static bool first_call = true;        /* initialise on first use    */

    /* ---------- Initialisation of candidate ---------- */
    if (first_call) {
        candidate = 2;                     /* start with the first prime */
        first_call = false;
    }

    /* ---------- Extend cache capacity if needed ---------- */
    if (cache_len < (size_t)n && cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure          */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---------- Generate terms until we reach n ---------- */
    while (cache_len < (size_t)n) {
        /* Skip non‑prime candidates fast: after 2 only odds */
        if (candidate > 2 && (candidate & 1) == 0) {
            candidate += 1;
            continue;
        }

        /* Test primality of candidate */
        if (is_prime_big(candidate)) {
            /* Check companion number 6*p – 7 */
            seq_t companion = candidate * 6 - 7;
            if (is_prime_big(companion)) {
                cache[cache_len++] = candidate;
            }
        }

        /* Advance candidate */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;                  /* jump to next odd */
    }

    return cache[n - 1];
}
