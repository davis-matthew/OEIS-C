/*
 * A087634  Primes p such that the equation phi(x) = 4p has a solution, where
 *          phi is the totient function.
 * https://oeis.org/A087634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A087634 is the sequence of primes p such that φ(x)=4p has a
 *            solution. For a prime p (p=2 or 2p+1 or 4p+1 prime) this holds
 * Range:     u8 n <= 31, u16 n <= 1581, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

static seq_t *term_list = NULL;     /* stored primes that satisfy the test */
static size_t term_count = 0;       /* number of such primes stored      */
static size_t term_cap   = 0;       /* current capacity of term_list     */
static idx_t   next_candidate = 2;  /* next integer to test for primality */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for sub‑offset indices */

    /* Allocate the list on first use. */
    if (term_list == NULL) {
        term_cap = 16;
        term_list = (seq_t *)malloc(term_cap * sizeof(seq_t));
    }

    /* Generate further terms until we have at least n of them. */
    while (term_count < n) {
        /* Find the next prime ≥ next_candidate. */
        idx_t p = next_candidate;
        if (p <= 2) {
            p = 2;
        } else if ((p & 1U) == 0) {
            p++;                     /* make it odd if it is even */
        }

        while (!isPrime(p)) {
            p += 2;                  /* skip even numbers */
        }

        /* Advance the global candidate for the subsequent call. */
        next_candidate = (p == 2) ? 3 : p + 2;

        /* Criterion: p=2 or 2p+1 or 4p+1 is prime. */
        if (p == 2 ||
            isPrime(2 * p + 1) ||
            isPrime(4 * p + 1)) {

            /* Grow the array if necessary. */
            if (term_count == term_cap) {
                term_cap <<= 1;               /* double the capacity */
                term_list = (seq_t *)realloc(term_list,
                                            term_cap * sizeof(seq_t));
            }
            term_list[term_count++] = (seq_t)p;
        }
        /* Continue looping until we have enough terms. */
    }

    /* Return the n‑th term (1‑based indexing). */
    return term_list[n - 1];
}
