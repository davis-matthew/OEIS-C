/*
 * A231190  Numerator of abs(n-8)/(2*n), n >= 1.
 * https://oeis.org/A231190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = abs(n-8)/gcd(n-8, 16)
 * Range:     u8 n <= 263, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t diff;
    if (n >= (idx_t)8)
        diff = (seq_t)(n - (idx_t)8);
    else
        diff = (seq_t)((idx_t)8 - n);

    seq_t g = gcd(diff, (seq_t)16);
    return diff / g;
}
