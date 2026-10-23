/*
 * A021733  Decimal expansion of 1/729.
 * https://oeis.org/A021733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/729) mod 10 Equivalent: let m = 10^(n+1)
 *            mod 7290; then a(n) = m / 729 (0 <= a(n) <= 9) This uses only
 *            integer arithmetic; no floating‑point or large numbers
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   pow10mod_7290
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10mod_7290.h"

seq_t a(idx_t n)
{
    /* 10^(n+1) mod 7290 */
    unsigned m = pow10mod_7290((uint64_t)n + 1ULL);
    /* 0 <= m < 7290, so m/729 is a single decimal digit */
    unsigned digit = m / 729U;
    return (seq_t)digit;
}
