/*
 * A021388  Decimal expansion of 1/384.
 * https://oeis.org/A021388
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

/* Implements calculation of the nth digit of the decimal expansion of 1/384
 * using long division.  The sequence a(n) is the (n+1)th digit starting
 * with a(0) = 0.  For n ≥ 0 the digit is:
 *   a(n) = floor( (10^(n+1) * 1) / 384 ) mod 10
 * This algorithm runs in O(n) time and uses only integer arithmetic.  It
 * does not store a lookup table of the terms. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder of the long division, never exceeds 383 */
    uint64_t rem = 1;
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;
        digit = (seq_t)(rem / 384ULL);
        rem %= 384ULL;
    }
    return digit;
}
