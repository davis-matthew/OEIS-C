/*
 * A161963  Even numbers n for which phi(n) > phi(n+1).
 * https://oeis.org/A161963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth even integer m such that φ(m) > φ(m+1)
 * Range:     u8 none, u16 n <= 526, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1; a(1) = 314, a(2) = 524, …) */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"

/* External linkage as required.  The types seq_t and idx_t are supplied
   by the build system.  No other definitions may be made. */
seq_t a(idx_t n)
{
    /* Static storage for the computed sequence values. */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static size_t terms_len  = 0;

    /* The next even integer to test.  Start at 2 and increase by 2 each
       iteration.  It keeps its value across calls. */
    static idx_t next_even = 2;

    /* Loop until we have at least n terms stored. */
    while (terms_len < n) {
        /* Compute φ for the even number and the following odd number. */
        seq_t phi_even = phi(next_even);
        seq_t phi_odd  = phi(next_even + 1);

        /* If the inequality holds, store the even number. */
        if (phi_even > phi_odd) {
            if (terms_len == terms_cap) {
                /* Grow the array: double the capacity or start at 16. */
                size_t new_cap = (terms_cap < 16) ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Out‑of‑memory: terminate the program. */
                    exit(EXIT_FAILURE);
                }
                terms      = tmp;
                terms_cap  = new_cap;
            }
            terms[terms_len++] = (seq_t)next_even;
        }

        /* Advance to the next even integer. */
        next_even += 2;
    }

    /* Return the requested 1‑based term. */
    return terms[n - 1];
}
