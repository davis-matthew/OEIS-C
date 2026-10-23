/*
 * A040810  Continued fraction for sqrt(839).
 * https://oeis.org/A040810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=28; for n>=1, a(n)=1 if n is odd (v2(n)=0), 27 if n has
 *            exactly one factor of 2 (v2(n)=1), otherwise 56
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;

    uint64_t temp = (uint64_t)n;
    uint64_t e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        e++;
    }

    if (e == 0)
        return (seq_t)1;
    else if (e == 1)
        return (seq_t)27;
    else
        return (seq_t)56;
}
