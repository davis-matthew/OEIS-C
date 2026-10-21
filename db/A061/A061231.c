/*
 * A061231  n - the reversal of n is a nonzero square.
 * https://oeis.org/A061231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer m such that m - rev(m) is a non‑zero
 *            perfect square
 * Range:     u8 n <= 10, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The code uses two helper utilities:
 *    1) rev(idx_t)   –  reverse the decimal digits of an idx_t number.
 *    2) floor_sqrt(seq_t) –  the integer square root of a seq_t number.
 *  These are provided in the tool‑chain via ../utils/rev.h and
 *  ../utils/floor_sqrt.h respectively.
 *
 *  No dynamic allocation or caching is required:  the function simply
 *  scans forward from 0 until the needed term is found, which is fast
 *  enough for n ≤ 10 000 on all supported builds.  For seq_t types
 *  that can hold the required value the algorithm stays correct.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/rev.h"            /* idx_t rev(idx_t) */
#include "../utils/floor_sqrt.h"     /* seq_t floor_sqrt(seq_t) */

/* Return true if x is a non‑zero perfect square. */
static inline bool is_square(seq_t x)
{
    if (x == 0) return false;
    seq_t r = floor_sqrt(x);
    return r * r == x;
}

/* ------------------------------------------------------------------
 *  a(n):  the n‑th number whose difference with its decimal reversal
 *         is a positive perfect square.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t found  = 0;            /* how many valid terms found so far */
    idx_t cand   = 0;            /* current integer candidate          */

    while (1) {
        /* Skip 0; its reversal equals itself and the difference is 0 */
        if (cand != 0) {
            idx_t rev_cand = rev(cand);
            if (rev_cand < cand) {               /* ensures positive diff */
                seq_t diff = (seq_t)(cand - rev_cand);
                if (is_square(diff)) {
                    if (found == n) {             /* n‑th term reached   */
                        return (seq_t)cand;
                    }
                    ++found;                       /* move to next target */
                }
            }
        }
        ++cand;
    }

    /* Unreachable – the loop will always return for valid input. */
}
