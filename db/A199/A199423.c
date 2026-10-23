/*
 * A199423  Greatest prime factor of n and 2*n+1.
 * https://oeis.org/A199423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(gpf(n), gpf(2*n+1))
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1, but guard */
    uint64_t nn   = (uint64_t)n;                /* convert index to 64‑bit value */
    uint64_t p1   = gpf(nn);                    /* greatest prime factor of n   */
    uint64_t p2   = gpf((uint64_t)(nn * 2 + 1));/* greatest prime factor of 2n+1 */
    return (seq_t)(p1 > p2 ? p1 : p2);
}
