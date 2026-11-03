/*
 * A178411  a(1)=2, a(2)=1; for n>=3, a(n) is defined by recursion:
 *          Sum_{d|n}((-1)^(n/d))*a(d) = -1.
 * https://oeis.org/A178411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=2, a(2)=1; for n>2, a(n)=n if n is a power of 2, otherwise
 *            a(n)=mu(n)
 * Range:     i8 n <= 127, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9992 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)1;
    if ((n & (n - 1)) == 0)          /* power of 2 */
        return (seq_t)n;
    return mobius(n);
}
