/*
 * A014701  Number of multiplications to compute n-th power by the
 *          Chandah-sutra method.
 * https://oeis.org/A014701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(log_2(n)) + popcount(n) - 1
 * Range:     u8 n <= 9154, u16 n <= 9154, u32 n <= 9154, u64 n <= 9154,
 *            u128 n <= 9154, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014701: Number of multiplications to compute the n-th power via the Chandah-sutra method */

#include <stdint.h>  /* for uint64_t */

seq_t a(idx_t n)
{
    /* n is at least 1, so clzll and popcount are defined */
    unsigned int log2 = 63 - __builtin_clzll(n);
    unsigned int pop   = __builtin_popcountll(n);
    return (seq_t)(log2 + pop - 1);
}
