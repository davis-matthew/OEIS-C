/*
 * A021659  Decimal expansion of 1/655.
 * https://oeis.org/A021659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) mod 6550) / 655 (the n-th decimal digit of
 *            1/655)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo 6550 and take the integer division by 655. */
    seq_t v = mod_pow((seq_t)10, (uint64_t)(n + 1), (seq_t)6550);
    return v / 655;
}
