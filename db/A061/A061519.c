/*
 * A061519  a(0) = 1; a(n) is obtained by incrementing each digit of a(n-1) by
 *          5.
 * https://oeis.org/A061519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (n even) ? (10^{2^{n/2}}-1)/9 : 6*(10^{2^{n/2}}-1)/9
 *            where the exponent 2^{n/2} is the number of repeated digits
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the explicit formulas stated for A061519. */

#include <stdint.h>
#include "../utils/pow10.h"   /* pow10(idx_t e) -> seq_t */

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Compute k = floor(n/2).  The exponent for 10 is 2^k. */
    idx_t k = n >> 1;
    /* 2^k fits in idx_t for typical test ranges.  */
    idx_t exp = 1ULL << k;   /* exponent for pow10 */
    seq_t p10 = pow10(exp);        /* 10^exp */
    seq_t base = (p10 - 1) / 9;    /* (10^exp - 1)/9 : repeated '1's */

    if ((n & 1) == 0) {           /* even n => all '1's */
        return base;
    } else {                      /* odd n => all '6's */
        return 6 * base;
    }
}
