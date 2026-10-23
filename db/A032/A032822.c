/*
 * A032822  Numbers whose set of base-10 digits is {1,4}.
 * https://oeis.org/A032822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_log2_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 4; 
   a(n) = 10 * a(floor(n/2)) + 1   for odd n;
   a(n) = 10 * a(floor((n-1)/2)) + 4   for even n. */

#include <stdint.h>
#include "../utils/floor_log2_custom.h"

seq_t a(idx_t n)
{
    /* n is 1‑based */
    uint64_t v = n + 1;                      /* ensure v >= 2 */
    uint64_t d = floor_log2_custom(v);       /* number of digits */
    /* compute start index of d‑digit block: 2^d - 1 */
    uint64_t pow2 = 1;
    for (uint64_t k = 0; k < d; ++k)
        pow2 *= 2ULL;
    uint64_t start = pow2 - 1;
    uint64_t offset = n - start;             /* 0 .. 2^d-1 */

    seq_t ans = 0;
    for (int64_t i = (int64_t)d - 1; i >= 0; --i)
    {
        uint64_t bit = (offset >> i) & 1ULL;
        seq_t digit = (bit ? (seq_t)4 : (seq_t)1);
        ans = ans * (seq_t)10 + digit;
    }
    return ans;
}
