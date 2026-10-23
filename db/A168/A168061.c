/*
 * A168061  Denominator of (n+3) / ((n+2) * (n+1) * n).
 * https://oeis.org/A168061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = denominator of (n+3)/((n+2)*(n+1)*n)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset does not use n=0                       */
    seq_t num   = n + 3;           /* numerator of the fraction                    */
    seq_t denom = (seq_t)n;        /* start building the denominator               */
    denom = denom * (n + 1);       /* multiply by (n+1)                            */
    denom = denom * (n + 2);       /* multiply by (n+2)                            */
    seq_t g = gcd(denom, num);     /* greatest common divisor                       */
    return denom / g;              /* reduced denominator                          */
}
