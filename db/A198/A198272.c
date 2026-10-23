/*
 * A198272  a(n) = round(n*sqrt(17)).
 * https://oeis.org/A198272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(17))
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 3353, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute a(n) without floating point by using the identity
   round(n*sqrt(17)) = (floor(2*n*sqrt(17)) + 1) / 2.  Since
   floor(2*n*sqrt(17)) = floor(sqrt(17 * (2*n)^2)), we can obtain it
   using the integer square‑root routine floor_sqrt. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t x  = nn * 2;            /* x = 2n */
    seq_t k  = 17 * x * x;        /* k = 17 * (2n)^2 = floor((2n*sqrt(17))^2) */
    seq_t m  = floor_sqrt(k);     /* m = floor(2n*sqrt(17)) */
    seq_t result = (m + 1) / 2;   /* round(n*sqrt(17)) */
    return result;
}
