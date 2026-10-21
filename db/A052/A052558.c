/*
 * A052558  a(n) = n! *((-1)^n + 2*n + 3)/4.
 * https://oeis.org/A052558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * ((-1)^n + 2*n + 3)/4 Recurrence: a(0)=1, a(1)=1,
 *            a(n)=a(n-1)+(n^2-1)*a(n-2) for n>=2 (O(n) time, memoised across
 *            calls)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *term = NULL;
    static size_t len = 0;   /* number of computed terms */
    static size_t cap = 0;   /* allocated capacity */

    /* initialise cache with first two terms */
    if (cap == 0) {
        cap = 4;
        term = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!term) return (seq_t)0;   /* out of memory */
        term[0] = (seq_t)1;           /* a(0) */
        term[1] = (seq_t)1;           /* a(1) */
        len = 2;
    }

    /* Extend cache up to n on demand */
    while (len <= (size_t)n) {
        idx_t i = (idx_t)len;
        seq_t val;
        if (len == 0) {              /* should not happen as we init with a(0) */
            val = (seq_t)1;
        } else if (len == 1) {       /* a(1) already set */
            val = (seq_t)1;
        } else {
            /* a(i) = a(i-1) + (i^2-1)*a(i-2) */
            seq_t mult = (seq_t)(i * i - 1);
            val = term[i-1] + (mult * term[i-2]);
        }

        /* Ensure capacity */
        if (len >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(term, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* out of memory */
            term = tmp;
            cap = newcap;
        }

        term[len++] = val;
    }

    return term[(size_t)n];
}
