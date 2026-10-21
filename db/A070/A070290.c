/*
 * A070290  a(n) = lcm(8,n)/gcd(8,n).
 * https://oeis.org/A070290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lcm(8,n) / gcd(8,n) = (n / gcd(8,n)) * (8 / gcd(8,n))
 * Range:     u8 n <= 32, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t type that supports the usual
   arithmetic operators and for any n ≥ 1. */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Compute gcd for two 64‑bit unsigned integers.  */

seq_t a(idx_t n)
{
    if (n == 0) /* offset guard – not used for valid sequence values */
        return (seq_t)0;
    uint64_t g = gcd_uint64(8ULL, n);           /* gcd(8, n)  */
    seq_t part1 = (seq_t)n / (seq_t)g;          /* n / g       */
    seq_t part2 = (seq_t)8 / (seq_t)g;          /* 8 / g       */
    return part1 * part2;                       /* product */
}
