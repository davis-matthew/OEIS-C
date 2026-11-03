/*
 * A089224  In binary representation: number of zeros of number of zeros of n.
 * https://oeis.org/A089224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A023416(A023416(n)) (number of zeros of the number of
 *            zeros of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 9567 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of zeros in the binary representation of n.
   For n = 0 the convention is to return 1 zero. */
static inline seq_t zeros_in_binary(idx_t n)
{
    if (n == 0)
        return 1;                     /* A023416(0) = 1 */
    uint64_t ones = __builtin_popcountll(n);
    uint64_t len  = 64 - __builtin_clzll(n);   /* bit length of n */
    uint64_t zeros = len - ones;
    return (seq_t)zeros;
}

seq_t a(idx_t n)
{
    seq_t z1 = zeros_in_binary(n);         /* A023416(n) */
    return zeros_in_binary((idx_t)z1);      /* A023416 of that result */
}
