/*
 * A027660  a(n) = C(n+2, 2) + C(n+2, 3) + C(n+2, 4) + C(n+2, 5).
 * https://oeis.org/A027660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,5) + binomial(n+3,3)
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 218, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 969 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* offset is 0, so use n directly */
    idx_t m = n + 3;                    /* we need C(m,5) + C(m,3) */

    seq_t term5 = binomial(m, 5);
    seq_t term3 = binomial(m, 3);

    return term5 + term3;
}
