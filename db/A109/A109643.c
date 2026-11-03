/*
 * A109643  a(1) = 1, a(n) = square of the concatenation of all previous
 *          terms.
 * https://oeis.org/A109643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = square of the concatenation of all previous terms
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   mem, num_digits0_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits0_seq.h"
#include "../utils/mem.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms and state of the concatenation */
    static seq_t *terms   = NULL;   /* array holding a(1) … a(size-1) */
    static idx_t  size    = 0;      /* number of terms stored */
    static idx_t  cap     = 0;      /* allocated capacity of terms */
    static seq_t  curcat   = 0;     /* concatenation of all terms so far */

    /* Initialise on first invocation */
    if (!terms) {
        cap    = 1;
        terms  = seq_malloc(cap);
        if (!terms) return 0;          /* allocation failure → zero */
        size   = 1;
        terms[0] = (seq_t)1;
        curcat = (seq_t)1;
    }

    /* If the requested term is already known, return it */
    if (n <= size)
        return terms[n-1];

    /* Expand cache up to n */
    while (size < n) {
        /* a(k+1) = (concatenation of first k terms)² */
        seq_t new_val = curcat * curcat;

        /* Ensure sufficient storage for the new term */
        if (size >= cap) {
            idx_t new_cap = cap ? cap * 2 : 1;
            seq_t *tmp = seq_realloc(terms, new_cap);
            if (!tmp) return 0;          /* reallocation failure → zero */
            terms = tmp;
            cap   = new_cap;
        }

        terms[size] = new_val;
        ++size;

        /* Update curcat: append the new term to the existing decimal
           representation.  curcat = curcat * 10^{digits(new_val)} + new_val */
        idx_t d   = num_digits0_seq(new_val);
        seq_t p10 = pow10(d);
        curcat = curcat * p10 + new_val;
    }

    return terms[n-1];
}
