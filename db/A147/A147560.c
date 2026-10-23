/*
 * A147560  a(n) = 4*A046162(n+1).
 * https://oeis.org/A147560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4 * numerator(n^2/(n^2 + 3*n + 3))
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn;
    seq_t den = num + nn * (seq_t)3 + (seq_t)3;
    seq_t g = gcd(num, den);
    seq_t rem = num / g;
    return rem * (seq_t)4;
}
