/*
 * A017770  Binomial coefficients C(54,n).
 * https://oeis.org/A017770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(54,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: full, pass, 55 terms through n = 54 (2026-11-02)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n > 54ULL) return 0;
    return binomial(54ULL, n);
}
