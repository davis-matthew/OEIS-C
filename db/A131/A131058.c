/*
 * A131058  List of numbers n whose reverse, R(n), is not less than n.
 * https://oeis.org/A131058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 5688, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer whose decimal reverse is not
   less than the integer itself (i.e., reverse(n) >= n).  The
   sequence starts 0,1,2,3,4,5,6,7,8,9,11,12,...  */

/* Standard headers for dynamic allocation. */
#include <stdlib.h>

/* Reverse a seq_t value in base 10.  Works for all supported seq_t
   types because it uses only unsigned arithmetic operators. */
static seq_t reverse_decimal(seq_t x)
{
    seq_t r = (seq_t)0;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)10;      /* last digit */
        r = r * (seq_t)10 + d;        /* append it */
        x = x / (seq_t)10;            /* drop the last digit */
    }
    return r;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *list = NULL;
    static idx_t len  = 0;          /* number of terms stored   */
    static idx_t cap  = 0;          /* allocated capacity      */
    static seq_t next = (seq_t)0;   /* next candidate to test  */

    /* Grow the cache until it contains at least n terms. */
    while (len < n) {
        /* Allocate a larger buffer if needed. */
        if (len >= cap) {
            idx_t new_cap = (cap == 0) ? 16 : cap * 2;
            /* Explicit cast for C++ compatibility. */
            seq_t *p = (seq_t *)realloc(list, (size_t)new_cap * sizeof(seq_t));
            if (!p) {            /* allocation failure: abort with 0. */
                return (seq_t)0;
            }
            list = p;
            cap  = new_cap;
        }

        /* Test the current candidate. */
        seq_t cand = next;
        seq_t rev  = reverse_decimal(cand);
        if (rev >= cand) {
            list[len++] = cand;    /* store if it satisfies the condition. */
        }
        next++;                    /* move to the next integer. */
    }

    /* Convert from 1‑based OEIS indexing to 0‑based array index. */
    return list[(size_t)n - 1];
}
