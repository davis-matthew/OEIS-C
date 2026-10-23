/*
 * A154436  Permutation of nonnegative integers induced by Lamplighter group
 *          generating wreath recursion, variant 1: a = s(a,b), b = (a,b),
 *          starting from the state a.
 * https://oeis.org/A154436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n ^ ((p - 1) - ((n - p) >> 1)), where p = 2^⌊log₂(n)⌋
 *            (the largest power of two ≤ n). For n = 0, a(0) = 0. This
 *            implements OEIS A154436
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   highest_pow2_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/highest_pow2_leq.h"

seq_t a(idx_t n)
{
    /* Base cases coincide with the recurrence for all n ≥ 1, but n = 0
       is handled explicitly. */
    if (n == 0) {
        return 0;
    }
    idx_t p = highest_pow2_leq(n);           /* p = 2^⌊log₂(n)⌋ */
    idx_t offset = n - p;
    idx_t half_offset = offset >> 1;
    seq_t xor_val = (seq_t)(p - 1) - (seq_t)half_offset;
    return (seq_t)n ^ xor_val;
}
