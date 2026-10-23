/*
 * A125667  Eta numbers (from the Japanese word for "pariah" or "outcast").
 *          These are the positive odd integers which cannot be used to make a
 *          hypotenuse of a primitive Pythagorean triangle (PPT).
 * https://oeis.org/A125667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_eta
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Eta numbers (OEIS A125667)
 *
 * An odd integer c is an eta number iff it has at least one prime factor
 * congruent to 3 (mod 4).  Equivalently, c is *not* a product of primes
 * all congruent to 1 (mod 4).  The function below searches successive odd
 * numbers until the n‑th eta number is found.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_eta.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* offset not used but harmless */

    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;   /* first odd integer */

    while (count < n) {
        if (is_eta(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += (seq_t)2;   /* advance to next odd integer */
    }

    /* This point is never reached for valid `n` ≥ 1. */
    return (seq_t)0;
}
