/*
 * A021345  Decimal expansion of 1/341.
 * https://oeis.org/A021345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * ((10^n) mod 341) / 341)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   powmod10_mod341
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th digit after the decimal point in the expansion of 1/341 */

#include <stdint.h>
#include "../utils/powmod10_mod341.h"

seq_t a(idx_t n)
{
    unsigned int r     = powmod10_mod341(n);
    unsigned int digit = (10U * r) / 341U;
    return (seq_t)digit;
}
