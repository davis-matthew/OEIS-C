/*
 * A135456  Number of surjections from an n-element set onto a seven-element
 *          set.
 * https://oeis.org/A135456
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   mul_small, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of surjections from an n‑element set onto a 7‑element set.
 * Formula: a(n) = Σ_{i=0}^{7} (-1)^i * C(7,i) * (7-i)^n   (n ≥ 7)
 * This implementation uses the inclusion–exclusion formula and the
 * utility functions pow_uint (base^exp) and mul_small (∞ * small int).
 * It is valid for any seq_t type: an unsigned integer of any width
 * or an arbitrary‑precision integer class (compiled as C++17).  It
 * runs in O(log n) time per term with negligible constant factors. */

#include <stddef.h>
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp);   */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 7.  For smaller n we
     * return 0 (the OEIS does not specify these values). */
    if (n < 7)
        return 0;

    /* Binomial coefficients C(7,i) for i = 0..7 */
    const unsigned int binom[8] = {1, 7, 21, 35, 35, 21, 7, 1};

    seq_t result = 0;

    for (idx_t i = 0; i <= 7; ++i)
    {
        seq_t term = mul_small(pow_uint(7 - i, n), binom[i]);

        /* Add if i is even, subtract if i is odd. */
        if (i & 1U)
            result = result - term;
        else
            result = result + term;
    }

    return result;
}
