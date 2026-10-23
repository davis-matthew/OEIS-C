/*
 * A164097  Numbers k such that 6*k + 7 is a perfect square.
 * https://oeis.org/A164097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 208, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164097: numbers k such that 6*k + 7 is a perfect square. */
/* a(n) is the n‑th smallest such integer.  The sequence is the sorted merge
   of the two strictly increasing quadratic progressions
   f₋(t) = 6t² - 2t - 1  and  f₊(t) = 6t² + 2t - 1  for t ≥ 1. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;            /* undefined for n < 1, return 0 */
    }

    /* indices (t) for the two progressions */
    idx_t t_minus = 1;
    idx_t t_plus  = 1;

    /* last produced term */
    idx_t last_val = 0;
    idx_t idx = 0;

    while (idx < n) {
        /* current values of each progression */
        idx_t val_minus = 6 * t_minus * t_minus - 2 * t_minus - 1;
        idx_t val_plus  = 6 * t_plus  * t_plus  + 2 * t_plus  - 1;

        if (val_minus <= val_plus) {
            last_val = val_minus;
            ++t_minus;
        } else {
            last_val = val_plus;
            ++t_plus;
        }
        ++idx;
    }

    return (seq_t)last_val;
}
