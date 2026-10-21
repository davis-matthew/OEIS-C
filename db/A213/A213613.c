/*
 * A213613  Decimal expansion of the duration of the Gregorian year in SI
 *          seconds.
 * https://oeis.org/A213613
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, cons, full, fini
 * Formula:   a(n) = 0 for n < 8 or n > 15, otherwise a(n) = (31556952 /
 *            10^(15-n)) % 10
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The digits of the integer 31556952 are returned sequentially,
   starting at n = 8, with trailing zeros thereafter. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n outside the valid range return 0. */
    if (n < 8 || n > 15)
        return (seq_t)0;

    /* pos runs from 0 (most significant digit) to 7 (least). */
    idx_t pos = n - 8;          /* 0 … 7 */
    idx_t power = 1;
    for (idx_t i = 0; i < 7 - pos; ++i)
        power *= 10u;           /* 10^(7-pos) */

    idx_t value = 31556952u / power;
    return (seq_t)(value % 10u);
}
