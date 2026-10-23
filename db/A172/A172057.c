/*
 * A172057  Primes p such that either p-3 or p-2 (but not both) is/are prime.
 * https://oeis.org/A172057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, changed
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 859, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) enumerates primes p where exactly one of p-2 or p-3 is prime */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* sequence is 1‑indexed; n≥1 per spec */

    /* static state for memoization */
    static seq_t *terms = NULL;     /* cached sequence values          */
    static size_t terms_cnt = 0;    /* number of cached terms          */
    static size_t terms_cap = 0;    /* current allocation capacity     */
    static idx_t last_checked = 1;  /* last integer that was tested   */

    /* extend cache until it contains n terms */
    while (terms_cnt < n) {
        /* search for next prime */
        if (++last_checked <= 1) last_checked = 2;   /* safety */
        if (!is_prime(last_checked)) continue;       /* composite */

        /* test the condition for p-2 / p-3 */
        bool p2 = (last_checked >= 3) && is_prime(last_checked - 2);
        bool p3 = (last_checked >= 4) && is_prime(last_checked - 3);
        if (p2 ^ p3) {   /* exactly one is prime */
            /* grow cache if necessary */
            if (terms_cnt == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return 0;          /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = (seq_t)last_checked;
        }
    }

    return terms[n - 1];
}
