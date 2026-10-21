/*
 * A070321  Greatest squarefree number <= n.
 * https://oeis.org/A070321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Max( core(k) : k = 1,2,3,…,n ) where core(k) is the
 *            square‑free part of k. The implementation finds the largest
 *            square‑free integer ≤ n by scanning backward from n until a
 *            square‑free number is found
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility that returns 1 iff x is square‑free. */
#include "../utils/is_squarefree.h"

/* Greatest square‑free number ≤ n.
   The result is defined for all n ≥ 1. */
seq_t a(idx_t n)
{
    /* If called with n == 0 the OEIS offset rules do not require a value.
       Return 0 as a harmless placeholder. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert the index once; the cast is safe because seq_t may be any
       unsigned width or an arbitrary‑precision integer. */
    seq_t k = (seq_t)n;

    /* Scan downward until a square‑free integer is found.
       There is always at least one (1). */
    for (;; --k) {
        if (is_squarefree(k)) {
            return k;
        }
    }
}
