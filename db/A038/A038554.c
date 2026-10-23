/*
 * A038554  Derivative of n: write n in binary, replace each pair of adjacent
 *          bits with their mod 2 sum (a(0)=a(1)=0 by convention). Also n XOR
 *          (n shift 1).
 * https://oeis.org/A038554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, nice, easy
 * Formula:   a(n) = (n ^ (n >> 1)) & ((1 << floor_log2(n)) - 1)
 * Range:     u8 n <= 262, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(0)=a(1)=0 by definition */

    /* floor_log2_uint64 returns floor(log2(n)) for n>0 */
    unsigned k = floor_log2_uint64(n);

    /* Build a mask with k lower bits set to 1.
       If k is larger than or equal to the width of seq_t,
       mask all bits.  This cannot happen for test cases that
       fit in seq_t. */
    seq_t mask;
    if (k >= sizeof(seq_t) * 8) {
        mask = (seq_t)-1;
    } else {
        mask = ((seq_t)1 << k) - 1;
    }

    seq_t val = ((seq_t)n ^ ((seq_t)n >> 1)) & mask;
    return val;
}
