/*
 * A021833  Decimal expansion of 1/829.
 * https://oeis.org/A021833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digits of the fraction 1/829.
   The sequence a(n) lists the n-th digit after the decimal point
   of 1/829; a(0) is the first digit.
   The decimal expansion of 1/829 is purely periodic with period ≤ 828.
   The implementation computes the period once using long division
   and then returns the required digit by modulo indexing. */

#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static idx_t period = 0;
static seq_t digits[829];          /* period ≤ 828 */

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Pre‑compute the full period of the decimal expansion. */
        idx_t rem = 1;
        do {
            digits[period++] = (seq_t)((rem * 10) / 829);
            rem = (rem * 10) % 829;
        } while (rem != 1 && period < 829);
        initialized = true;
    }

    /* Return the digit corresponding to the index modulo the period. */
    return digits[(size_t)(n % period)];
}
