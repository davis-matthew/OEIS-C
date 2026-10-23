/*
 * A046386  Products of exactly four distinct primes.
 * https://oeis.org/A046386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4317, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046386: Products of exactly four distinct primes (square‑free numbers
   with omega(n)=4). This implementation builds a table of all such numbers up to
   a fixed limit on the first call and returns the nth term from that table.
   The limit is chosen large enough to contain at least the first 10 000 terms. */

/* Helper allocation function for seq_t objects (available in the build) */
#include "../utils/seq_alloc_new_array.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of the sequence values */
static seq_t *prod_terms = NULL;   /* array of values */
static size_t prod_cnt   = 0;       /* number of elements stored */

/* Build the table of all products of exactly four distinct primes up to LIMIT. */
static void build_terms(void)
{
    const idx_t LIMIT = 200000;          /* safe upper bound for the first 10 000 terms */

    /* arrays for counting distinct prime factors and detecting square‑free numbers */
    uint8_t *omega   = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    uint8_t *sqfree  = (uint8_t *)calloc((size_t)LIMIT + 1, sizeof(uint8_t));
    if (!omega || !sqfree) abort();

    /* initially every integer is treated as square‑free */
    for (idx_t i = 0; i <= LIMIT; ++i) {
        sqfree[i] = 1;
    }

    /* sieve to compute ω(n) – the number of distinct prime factors of n */
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (omega[i] == 0) {                       /* i is prime */
            for (idx_t j = i; j <= LIMIT; j += i) {
                omega[j] += 1;
            }
        }
    }

    /* mark numbers that are not square‑free */
    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (omega[p] == 1) {                       /* p is prime */
            idx_t sq = p * p;
            for (idx_t m = sq; m <= LIMIT; m += sq) {
                sqfree[m] = 0;
            }
        }
    }

    /* count how many numbers satisfy ω(n)=4 and are square‑free */
    prod_cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (omega[i] == 4 && sqfree[i]) {
            ++prod_cnt;
        }
    }

    /* allocate the sequence array (properly constructed objects) */
    prod_terms = seq_alloc_new_array(prod_cnt);
    if (!prod_terms) abort();

    /* fill the array with the qualifying numbers in ascending order */
    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (omega[i] == 4 && sqfree[i]) {
            prod_terms[idx++] = (seq_t)i;   /* implicit conversion to seq_t */
        }
    }

    free(omega);
    free(sqfree);
}

/* Return the nth product of exactly four distinct primes.
   The OEIS offset is 1: a(1) is the first term.  For n==0 the function
   returns 0 as a sentinel. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                /* offset guard */
    }

    if (prod_terms == NULL) {    /* lazily build the table on first use */
        build_terms();
    }

    if ((n - 1) < prod_cnt) {
        return prod_terms[n - 1];
    } else {
        /* If n exceeds the precomputed range, we could rebuild with a larger
           LIMIT here.  For the intended usage (n ≤ 10 000) this branch is
           never taken. */
        return 0;
    }
}
