/*
 * A000221  Take sum of squares of digits of previous term; start with 5.
 * https://oeis.org/A000221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(1) = 5, a(n+1) = A003132(a(n)) (sum of squares of the digits
 *            of a(n)); ultimately periodic with period 8 for n >= 5
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   a000221_ssd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000221_ssd.h"

seq_t a(idx_t n)
{
    unsigned v = 5;
    idx_t i;
    if (n < 1)
        n = 1;
    if (n > 12)
        n = 5 + (n - 5) % 8;
    for (i = 1; i < n; i++)
        v = a000221_ssd(v);
    return (seq_t)v;
}
