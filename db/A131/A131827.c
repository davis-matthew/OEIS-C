/*
 * A131827  Numbers k such that cototient(x) = k has exactly 2 solutions.
 * https://oeis.org/A131827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that cototient(x)=k has exactly 2
 *            solutions
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *phi   = NULL;
static uint32_t *freq  = NULL;
static seq_t   *terms = NULL;
static size_t   terms_size = 0;
static idx_t    last_limit = 0;
static bool     initialized = false;

/* Recompute data up to the given 'limit'.
   Allocates the necessary arrays, builds the totient sieve,
   counts frequencies of cototients, and gathers the list of
   k’s that occur exactly twice. */
static void recompute(idx_t limit)
{
    phi   = (uint32_t *)realloc(phi,   (limit + 1) * sizeof(uint32_t));
    freq  = (uint32_t *)realloc(freq,  (limit + 1) * sizeof(uint32_t));
    terms = (seq_t   *)realloc(terms,  (limit + 1) * sizeof(seq_t));
    if (!phi || !freq || !terms)
        exit(1);        /* allocation failed */

    /* Euler's totient sieve. */
    for (idx_t i = 0; i <= limit; ++i)
        phi[i] = (uint32_t)i;
    if (limit >= 1) phi[1] = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (phi[p] == (uint32_t)p) {          /* p is prime */
            for (idx_t j = p; j <= limit; j += p)
                phi[j] -= phi[j] / (uint32_t)p;
        }
    }

    /* Frequency of cototient values. */
    memset(freq, 0, (limit + 1) * sizeof(uint32_t));
    for (idx_t x = 2; x <= limit; ++x) {
        uint32_t k = (uint32_t)(x - phi[x]);  /* cototient(x) */
        if (k <= limit)
            ++freq[k];
    }

    /* Build list of k with exactly two preimages. */
    terms_size = 0;
    for (uint32_t k = 1; k <= limit; ++k) {
        if (freq[k] == 2)
            terms[terms_size++] = (seq_t)k;   /* cast to seq_t */
    }

    last_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* outside OEIS range */

    if (!initialized) {
        initialized = true;
        recompute(1000000);                /* initial bound */
    }

    /* Increase bound until we have enough terms. */
    while (terms_size < (size_t)n) {
        recompute(last_limit * 2);
    }

    return terms[n - 1];
}
