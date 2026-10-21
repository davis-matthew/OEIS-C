/*
 * A071698  Lesser members of twin prime pairs of form (4*k+3, 4*k+5), k >= 0.
 * https://oeis.org/A071698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th lesser member of twin prime pairs (4k+3,4k+5)
 * Range:     u8 n <= 9, u16 n <= 437, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches for the next pair, caches all found terms and
   uses the provided isPrime(idx_t) routine for primality testing.  It works
   for any seq_t and idx_t defined by the build, including arbitrary‑precision
   seq_t, because the numbers we generate fit into idx_t (uint64_t). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* State for incremental computation */
static idx_t  last_k  = 0;          /* next k to try for 4k+3 */
static seq_t *terms   = NULL;       /* array of computed terms       */
static size_t terms_len = 0;        /* number of terms stored        */
static size_t terms_cap = 0;        /* allocated capacity of terms    */

/* Ensure capacity for another term */
static void push_term(seq_t v)
{
    if (terms_cap <= terms_len) {
        terms_cap = (terms_cap == 0) ? 4 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        terms = tmp;                /* assume memory allocation never fails */
    }
    terms[terms_len++] = v;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; n==0 is undefined but we return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        idx_t candidate = 4 * last_k + 3;   /* 4k+3 */
        /* Check twin prime condition using the 64‑bit primality routine. */
        if (isPrime(candidate) && isPrime(candidate + 2))
            push_term((seq_t)candidate);     /* store as a(n)            */
        ++last_k;
    }

    return terms[n - 1];
}
