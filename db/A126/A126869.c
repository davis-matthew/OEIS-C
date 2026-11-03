/*
 * A126869  a(n) = Sum_{k = 0..n} binomial(n,floor(k/2))*(-1)^(n-k).
 * https://oeis.org/A126869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, n/2) for even n, a(n) = 0 for odd n
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 924 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n & 1)
        return 0;               /* odd indices give 0 */
    return binomial(n, n/2);     /* central binomial coefficient */
}
