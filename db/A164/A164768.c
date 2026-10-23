/*
 * A164768  First differences of A003592.
 * https://oeis.org/A164768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003592(n+1) – A003592(n) where A003592 lists the numbers
 *            of the form 2^i * 5^j in increasing order
 * Range:     u8 n <= 31, u16 n <= 88, u32 n <= 290, u64 n <= 1087,
 *            u128 n <= 3943, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *terms = NULL;   /* array of A003592 terms, 0‑based */
    static size_t  capacity = 0;  /* allocated capacity of terms */
    static idx_t  last_index = 0; /* highest index currently generated (0‑based) */
    static idx_t  idx2 = 0;       /* pointer for multiples of 2 */
    static idx_t  idx5 = 0;       /* pointer for multiples of 5 */

    /* initialise cache on first call */
    if (terms == NULL) {
        capacity  = 1;
        terms     = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!terms) abort();            /* fatal if allocation fails */
        terms[0]  = (seq_t)1;           /* 2^0 * 5^0 = 1 */
        last_index = 0;
        idx2 = 0;
        idx5 = 0;
    }

    /* grow cache if needed: ensure we can store indices up to n (inclusive) */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal if reallocation fails */
        terms = tmp;
        capacity = new_cap;
    }

    /* generate terms until we have term[n] ready */
    while (last_index < n) {
        seq_t cand2 = terms[idx2] * ((seq_t)2);
        seq_t cand5 = terms[idx5] * ((seq_t)5);

        if (cand2 <= cand5) {
            terms[last_index + 1] = cand2;
            idx2++;
            if (cand2 == cand5) idx5++;   /* avoid duplicates */
        } else {
            terms[last_index + 1] = cand5;
            idx5++;
            if (cand2 == cand5) idx2++;   /* safe but unlikely */
        }
        last_index++;
    }

    /* return the difference a(n) = term[n] – term[n-1] */
    return terms[n] - terms[n-1];
}
