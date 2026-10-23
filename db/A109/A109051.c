/*
 * A109051  a(n) = lcm(n,10).
 * https://oeis.org/A109051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,10) = n*10/gcd(n,10)
 * Range:     u8 n <= 26, u16 n <= 6556, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t g  = seq_gcd(sn, (seq_t)10);
    if (g == (seq_t)0)   /* only when sn==0 */
        return (seq_t)0;
    return sn * (seq_t)10 / g;
}
