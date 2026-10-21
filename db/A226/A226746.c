/*
 * A226746  Numbers n such that x^2 = 1 has more than two solutions in the
 *          Gaussian integers modulo n.
 * https://oeis.org/A226746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   has_extra_solutions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th positive integer such that in
   the Gaussian integers modulo that integer the equation x² = 1
   has more than two solutions.  */
/* The criterion is:
     * n divisible by 4  →  true
     * n divisible by 2 but not 4  → true unless n == 2
     * otherwise (odd n):  true iff
            (n has a prime factor ≡ 1 (mod 4))  or
            (n has at least two distinct prime factors ≡ 3 (mod 4))  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_extra_solutions.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;            /* number of found terms   */
    uint64_t m   = 2;         /* candidate integer      */

    while (cnt < n) {
        if (has_extra_solutions(m)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)m;
        }
        ++m;
    }
    /* Control never reaches here for valid inputs */
    return (seq_t)0;
}
