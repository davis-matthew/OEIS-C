/*
 * A144795  A positive integer n is included if every 1 in binary n is next to
 *          at least one other 1.
 * https://oeis.org/A144795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 5841, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_isolated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A144795 – numbers whose binary representation has no isolated 1s.
   The function returns the n‑th smallest such integer (n ≥ 1).  A
   straightforward search with memoisation is used, which works for
   any integer type seq_t (including arbitrary‑precision) and any
   index type idx_t (uint64_t).  
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_isolated.h"

/*--------------------------------------------------------------------
 * Public function: a(idx_t n) – nth term of the sequence (1‑based).
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *terms  = NULL;      /* array of found terms              */
    static size_t term_cap = 0;       /* capacity of the array             */
    static size_t term_len = 0;       /* number of terms stored            */
    static idx_t  tested = 0;         /* last integer that was examined    */

    /* The sequence is defined only for n ≥ 1.  For safety return 0
       if the caller passes n == 0; this case is unused in the tests. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (term_len < n) {
        seq_t candidate = (seq_t)(tested + 1);   /* next integer to test */

        if (!has_isolated(candidate)) {          /* accept the term      */
            /* Grow the array if necessary. */
            if (term_len >= term_cap) {
                size_t new_cap = term_cap ? term_cap * 2 : 16;
                if (new_cap < term_len + 1) new_cap = term_len + 1;
                /* realloc returns void*; cast for C++ compatibility. */
                seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* allocation failure */
                terms = tmp;
                term_cap = new_cap;
            }
            terms[term_len++] = candidate;
        }
        ++tested;
    }

    return terms[n - 1];
}
