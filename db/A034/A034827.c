/*
 * A034827  a(n) = 2*binomial(n,4).
 * https://oeis.org/A034827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(n,4)
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 477, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 970 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t val = binomial(n, 4);
    return val + val;   /* equivalent to 2 * val */
}
