/*
 * A200511  Numbers n with omega(n)=2 and bigomega(n)>2, where
 *          omega=A001221=number of distinct prime factors,
 *          bigomega=A001222=prime factors counted with multiplicity.
 * https://oeis.org/A200511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bigomega, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A200511: numbers n with ω(n)=2 and Ω(n)>2.
 * The function generates terms by scanning integers and
 * accepting those with exactly two distinct prime factors and
 * a total prime factor multiplicity greater than two. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"

/* Test whether a given 64‑bit integer satisfies the condition. */
static bool qualifies(uint64_t x)
{
    return (count_distinct_primes(x) == 2) && (bigomega(x) > (seq_t)2);
}

/* Dynamic array to store the generated terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t next_scan = 2;   /* next integer to test */

/* Grow the array when more capacity is needed. */
static void grow_terms(void)
{
    if (terms_cap == 0) {
        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len == terms_cap) {
        size_t new_cap = terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n<1, return 0 */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        next_scan++;
        if (qualifies(next_scan)) {
            grow_terms();
            terms[terms_len++] = (seq_t)next_scan;
        }
    }
    return terms[n - 1];
}
