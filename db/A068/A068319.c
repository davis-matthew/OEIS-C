/*
 * A068319  a(n) = if n <= lpf(n)^2 then lpf(n) else a(lpf(n) + n/lpf(n)),
 *          where lpf = least prime factor, A020639.
 * https://oeis.org/A068319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = if n <= lpf(n)^2 then lpf(n) else a(lpf(n) + n/lpf(n)),
 *            a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lpf.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t n64 = (uint64_t)n;
    uint64_t p64  = (uint64_t)lpf(n);

    /* n <= lpf(n)^2  ⇔  n/lpf(n) <= lpf(n) for integer n/lpf(n) */
    if (n64 / p64 <= p64)
        return (seq_t)p64;

    return a((idx_t)(p64 + n64 / p64));
}
