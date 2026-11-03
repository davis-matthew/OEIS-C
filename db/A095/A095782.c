/*
 * A095782  Values of n for which A095777(n) is 13 (those terms which are
 *          expressible in decimal digits for bases 2 through 14, but not for
 *          base 15).
 * https://oeis.org/A095782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth positive integer that is
   representable in decimal digits for bases 2 through 14,
   but not representable in base 15.  This is equivalent to
   the description of OEIS sequence A095782. */

#include <stdbool.h>

/* Check that all digits of x in the given base are < 10. */
static bool digits_lt10(seq_t x, uint32_t base)
{
    while (x > 0) {
        if ((x % base) >= 10)
            return false;
        x /= base;
    }
    return true;
}

/* Test whether x satisfies the required property. */
static bool passes_property(seq_t x)
{
    /* Bases 11 to 14 must have only digits 0–9. */
    if (!digits_lt10(x, 11)) return false;
    if (!digits_lt10(x, 12)) return false;
    if (!digits_lt10(x, 13)) return false;
    if (!digits_lt10(x, 14)) return false;

    /* Base 15 must be *not* representable using only digits
       0–9, i.e., at least one digit must be ≥ 10. */
    if (digits_lt10(x, 15)) return false;

    return true;
}

/* Return the nth element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    idx_t   count = 0;      /* how many valid numbers found so far */
    seq_t   x = 1;          /* current number being tested */

    while (1) {
        if (passes_property(x)) {
            count++;
            if (count == n)
                return x;
        }
        ++x;               /* advance to next candidate */
    }
}
