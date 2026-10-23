/*
 * A126560  a(n) = gcd(4*(n+1)*(n+2), n*(n+3)), periodic with 8-cycle
 *          4,2,2,4,8,2,2,8.
 * https://oeis.org/A126560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(4*(n+1)*(n+2), n*(n+3))
 * Range:     u8 n <= 10, u16 n <= 1007, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (seq_t)4 * (nn + 1) * (nn + 2);
    seq_t t2 = nn * (nn + 3);
    return gcd(t1, t2);
}
