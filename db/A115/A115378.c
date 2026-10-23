/*
 * A115378  a(n) = number of positive integers k < n such that n XOR k =
 *          (n+k).
 * https://oeis.org/A115378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(number of 0's in binary expansion of n) - 1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for the OEIS sequence A115378.
 * n is an unsigned integer of type idx_t (uint64_t).
 * seq_t may be any unsigned integer type supported by the build.
 */
seq_t a(idx_t n)
{
    /* Fallback for n == 0: sequence offset starts at 1. */
    if (n == 0u)
        return (seq_t)0;

    /* Count zeros in the binary representation of n. */
    idx_t tmp = n;
    unsigned zeros = 0U;
    while (tmp != 0u) {
        if ((tmp & 1u) == 0u)
            ++zeros;
        tmp >>= 1u;
    }

    /* a(n) = 2^zeros - 1 */
    seq_t result = ((seq_t)1 << zeros) - 1;
    return result;
}
