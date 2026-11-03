/*
 * A186245  a(n) = binomial(n^2, 2*n).
 * https://oeis.org/A186245
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n^2, 2*n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 195 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset, the value is irrelevant. */
    if (n < 2) return 0;
    return binomial(n * n, 2 * n);
}
