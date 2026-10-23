/*
 * A040685  Continued fraction for sqrt(712).
 * https://oeis.org/A040685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 26. For n > 0, a(n) repeats the pattern 1, 2, 6, 2, 1,
 *            52. Thus a(n) = period[(n-1) mod 6] where period = {1, 2, 6, 2,
 *            1, 52}
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A040685: Continued fraction coefficients for sqrt(712). */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)26;

    /* The period of the continued‑fraction expansion of sqrt(712). */
    static const seq_t period[6] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)6,
        (seq_t)2,
        (seq_t)1,
        (seq_t)52
    };

    /* Compute (n-1) mod 6.  n>0, so n-1 fits in idx_t. */
    size_t idx = (size_t)((n - 1) % 6);
    return period[idx];
}
