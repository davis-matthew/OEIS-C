/*
 * A201147  Numbers m such that m, m-1 and m-2 are 1,2,3-almost primes
 *          respectively.
 * https://oeis.org/A201147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 167, u32 n <= 8400, u64 n <= 8400,
 *            u128 n <= 8400, big any n
 * Helpers:   bigomega, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A201147.  There is no known closed form, so the
   function searches for integers m in increasing order that satisfy
   the following conditions:
      1. m is prime  (1‑almost prime)
      2. m‑1 has exactly two prime factors counting multiplicity
         (2‑almost prime)
      3. m‑2 has exactly three prime factors counting multiplicity
         (3‑almost prime)
   Candidates are taken in the natural order m = 12·k + 11 (i.e. m ≡
   11 (mod 12)), which guarantees m is of the form 12k−1 as noted in the
   OEIS entry.  The function keeps a static cache so that calls with
   arbitrary n are answered in O(n) time where n is the requested index. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n);          */
#include "../utils/bigomega.h"      /* seq_t bigomega(idx_t n);         */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;               /* undefined for indices < 1          */
    }

    /* Static cache of computed terms. */
    static seq_t *terms   = NULL;
    static size_t count   = 0;       /* number of stored terms            */
    static size_t capacity = 0;     /* allocated capacity                */
    static idx_t  next_k = 0;       /* next k value for generating m     */

    /* Allocate the cache on first use. */
    if (!terms) {
        capacity = 16;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!terms) {
            return 0;   /* allocation failure, return 0 as a safe default */
        }
    }

    /* Generate terms until we have at least n terms. */
    while (count < n) {
        /* Candidate m = 12·k + 11. */
        idx_t cand = 12 * next_k + 11;

        /* Test the three conditions. */
        if (is_prime(cand) &&
            bigomega(cand - 1) == 2 &&
            bigomega(cand - 2) == 3) {

            /* Grow the cache if necessary. */
            if (count + 1 > capacity) {
                capacity *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure; give up. */
                    free(terms);
                    terms = NULL;
                    return 0;
                }
                terms = tmp;
            }

            /* Store the found term. */
            terms[count] = (seq_t)cand;
            count++;
        }
        next_k++;   /* advance to the next candidate */
    }

    return terms[n - 1];
}
