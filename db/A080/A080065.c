/*
 * A080065  Numbers k such that k == 3 modulo (lpf(k)+1), where lpf(k) is the
 *          least prime dividing k (A020639).
 * https://oeis.org/A080065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k such that k ≡ 3 (mod lpf(k)+1),
   where lpf(k) is the least prime factor of k. */

#include <stdlib.h>
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    /* Undefined for n <= 0, return 0. */
    if (n == 0) return (seq_t)0;

    /* Static cache of the generated terms. */
    static seq_t *terms = NULL;      /* array of terms */
    static size_t  cnt   = 0;        /* number of terms stored */
    static size_t  cap   = 0;        /* current capacity */
    static idx_t   cand  = 3;        /* next integer to test */

    /* Allocate an initial buffer if needed. */
    if (terms == NULL) {
        cap = 64;
        terms = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (terms == NULL) abort();  /* allocation failure */
    }

    /* Generate terms until we have at least n of them. */
    while (cnt < (size_t)n) {
        /* Check the congruence condition. */
        if (cand % (lpf(cand) + 1) == 3) {
            /* Grow the array if it is full. */
            if (cnt == cap) {
                cap <<= 1;  /* double the capacity */
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (tmp == NULL) abort();  /* allocation failure */
                terms = tmp;
            }
            terms[cnt++] = (seq_t)cand;
        }
        ++cand;  /* move to the next integer */
    }

    return terms[cnt - 1];
}
