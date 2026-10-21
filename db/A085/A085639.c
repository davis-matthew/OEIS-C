/*
 * A085639  Ramanujan sum c_n(5).
 * https://oeis.org/A085639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = μ(n) if 5∤n; otherwise a(n) = μ(n) + 5 · μ(n/5)
 * Range:     i8 n <= 9808, i16 n <= 9808, i32 n <= 9808, i64 n <= 9808,
 *            i128 n <= 9808, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided mobius routine from the utils library. */
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n % 5 == 0) {
        /* n divisible by 5: use the closed form derived from the
           definition of the Ramanujan sum cₙ(5) */
        return mobius(n) + 5 * mobius(n / 5);
    }
    /* n not divisible by 5: the Ramanujan sum equals μ(n) */
    return mobius(n);
}
