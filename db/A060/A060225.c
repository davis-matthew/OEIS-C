/*
 * A060225  Prices of Monopoly properties starting with Mediterranean Avenue
 *          and ending with Boardwalk. Does not include railroads or
 *          utilities.
 * https://oeis.org/A060225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Prices of Monopoly properties (excluding railroads and utilities).
   Values are defined for 1 ≤ n ≤ 22. For n > 22 this implementation returns 0. */

#include <stddef.h>   /* for size_t */

static const seq_t property_prices[22] = {
    60, 60, 100, 100, 120, 140, 140, 160, 180, 180,
    200, 220, 220, 240, 260, 260, 280, 300, 300, 320,
    350, 400
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n < 1, offset is 1 */
    if (n <= 22) return property_prices[(size_t)(n - 1)];
    return 0;                   /* outside defined range */
}
