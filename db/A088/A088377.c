/*
 * A088377  a(n) = (smallest prime factor of n)^2; a(1) = 1.
 * https://oeis.org/A088377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (smallest prime factor of n)^2; a(1)=1
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   spf_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/spf_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                /* a(1)=1 and a(0) is unused */
    uint64_t p = spf_uint64((uint64_t)n);
    seq_t sp = (seq_t)p;                 /* promote to result type */
    return sp * sp;                      /* square of the smallest prime factor */
}
