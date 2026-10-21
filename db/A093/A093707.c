/*
 * A093707  Numbers n such that n^3 + (n-1)^2 is a prime.
 * https://oeis.org/A093707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 13, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that n^3 + (n-1)^2 is prime.
   Computation: brute‑force search with memoisation.  For each new
   requested index we extend a cache of the terms that satisfy the
   condition, testing successive values of n and using the supplied
   is_prime_big() routine to test primality of the candidate
   value n^3 + (n-1)^2.  The cached terms are stored as seq_t,
   so the function works for all supported build configurations. */

#include <stdlib.h>
#include <stdbool.h>

/* The testing function is provided by the build system. */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;      /* number of cached terms */
    static size_t terms_cap = 0;      /* allocated capacity */
    static idx_t next_n = 1;          /* next integer to test */

    if (n == 0)                 /* not defined for n < 1 */
        return (seq_t)0;

    /* Grow the cache until it contains the nth term. */
    while (terms_len < (size_t)n) {
        /* Ensure enough space. */
        if (terms_len == terms_cap) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* allocation failed */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Test whether next_n satisfies the prime condition. */
        seq_t nseq = (seq_t)next_n;
        seq_t cand = nseq * nseq * nseq + (nseq - 1) * (nseq - 1);
        if (is_prime_big(cand))
            terms[terms_len++] = nseq;   /* store the integer n */

        ++next_n;
    }

    /* Return the requested term (1‑based index). */
    return terms[n - 1];
}
