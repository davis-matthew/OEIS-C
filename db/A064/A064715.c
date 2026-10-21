/*
 * A064715  Smallest member of triple of consecutive numbers each of which is
 *          the sum of two different nonzero squares.
 * https://oeis.org/A064715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 262, u32 n <= 1200, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   build_repr, grow_terms_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A064715
 *  a(n) = smallest integer m such that
 *         m, m+1 and m+2 are each a sum of two different
 *         non‑zero squares
 *
 *  Implementation:
 *  ---------
 *  We keep a growing boolean array `repr` where repr[x] == 1
 *  iff x can be expressed as a^2 + b^2 with 0 < a < b.
 *  Whenever the sieve limit is exceeded we double it and
 *  recompute the representation from scratch (the cost is
 *  negligible for the required range).
 *
 *  While searching, every time we find a triple (m,m+1,m+2)
 *  that satisfies the condition we store m in a dynamically
 *  growable array `terms`.
 *  The function returns terms[n‑1] (since the OEIS offset is 1).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_repr.h"
#include "../utils/grow_terms_array.h"

/* ---------- main OEIS function ------------------------------------- */
seq_t a(idx_t n)
{
    /* static state */
    static uint8_t *repr          = NULL;   /* sieve array          */
    static size_t repr_limit      = 0;      /* current sieve limit  */
    static seq_t *terms           = NULL;   /* found terms          */
    static size_t terms_cap       = 0;      /* capacity of terms    */
    static size_t terms_cnt       = 0;      /* number of terms known*/
    static size_t current_pos     = 1;      /* next candidate to test */

    /* initial allocation for terms array */
    if (terms == NULL) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    }

    /* build initial sieve */
    if (repr == NULL) {
        size_t init_limit = 2000000;        /* start with a moderate limit */
        build_repr(init_limit, &repr, &repr_limit);
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {

        /* extend sieve if we run out of range */
        while (current_pos + 2u > repr_limit) {
            size_t new_limit = repr_limit * 2;
            build_repr(new_limit, &repr, &repr_limit);
        }

        /* check the triple */
        if (repr[current_pos] && repr[current_pos + 1] && repr[current_pos + 2]) {
            /* store the term, grow array if necessary */
            if (terms_cnt == terms_cap) {
                grow_terms_array(&terms, &terms_cap, terms_cap * 2);
            }
            terms[terms_cnt++] = (seq_t)current_pos;
        }
        ++current_pos;
    }

    return terms[n - 1];
}
