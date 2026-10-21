/*
 * A071288  Denominators of Peirce sequence of order 6.
 * https://oeis.org/A071288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   periodic with period 21 a(n) = a(n mod 21), a(0..20) =
 *            {2,4,6,5,3,1,6,5,4,6,3,5,4,6,2,5,6,3,4,5,6} (derived from the
 *            generating function (6x^20 + ⋯ + 2) / (1 – x^21) )
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Twelve‑element period of the Peirce sequence of order 6 */
    static const seq_t period[21] = {
        2, 4, 6, 5, 3, 1, 6, 5, 4, 6, 3, 5,
        4, 6, 2, 5, 6, 3, 4, 5, 6
    };
    return period[(size_t)(n % 21)];
}
