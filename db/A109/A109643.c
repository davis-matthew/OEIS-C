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
 * Helpers:   digit_len, pow10, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

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
        idx_t d   = digit_len(new_val);
        seq_t p10 = pow10(d);
        curcat = curcat * p10 + new_val;
    }

    return terms[n-1];
}
