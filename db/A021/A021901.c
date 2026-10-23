/*
 * A021901  Decimal expansion of 1/897.
 * https://oeis.org/A021901
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

/* a(n) = digit of the n-th position after the decimal point in 1/897
   Obtained by long division: start with remainder 1 and advance it
   n times, then take the quotient of the next step. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* current remainder of the division 1/897 */
    uint64_t rem = 1ULL;

    /* advance the remainder n times (skip the first n digits) */
    for (idx_t i = 0; i < n; ++i)
        rem = (rem * 10ULL) % 897ULL;

    /* the next step gives the digit at position n */
    return (seq_t)((rem * 10ULL) / 897ULL);
}
