/*
 * A081802  Decimal expansion of electron charge 1.602177330000*10-19
 *          (Coulomb).
 * https://oeis.org/A081802
 *
 * Offset:    -18    (a(-18) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the (n+18)-th digit of the decimal expansion of
 *            1.602177330000×10⁻¹⁹; digits beyond the 13th are 0. a(-18) = 1,
 *            a(-17) = 6, …, a(-6) = 0, a(n) = 0 for n > -6
 * Range:     i8 n <= -10, i16 n <= -10, i32 n <= -10, i64 n <= -10,
 *            i128 n <= -10, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const char ED_DEC_DIGITS[] = "1602177330000";

seq_t a(idx_t n)
{
    /* For indices below the offset, behavior is irrelevant. */
    if (n < -18)
        return (seq_t)0;

    /* Translate n to a zero‑based index into the digit string. */
    idx_t pos = n + 18;  /* pos ranges from 0 to 12 for valid digits */

    /* For positions past the known digits, return 0 (decimal zeros). */
    if (pos < (idx_t)sizeof(ED_DEC_DIGITS) - 1)  /* exclude terminating NULL */
        return (seq_t)(ED_DEC_DIGITS[(int)pos] - '0');

    return (seq_t)0;
}
