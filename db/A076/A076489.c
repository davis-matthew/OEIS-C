/*
 * A076489  Number of common (distinct) digits of consecutive natural numbers.
 * https://oeis.org/A076489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_mask_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076489: Number of common (distinct) decimal digits of n and n+1.
   a(n) = |{d : d in digits(n) and digits(n+1)}|   for n ≥ 0
*/
#include <stdint.h>
#include "../utils/digits_mask_uint64.h"

seq_t a(idx_t n)
{
    uint64_t n1 = n + 1;                /* unsigned wrap works fine */
    unsigned int m0 = digits_mask_uint64((uint64_t)n);
    unsigned int m1 = digits_mask_uint64(n1);
    unsigned int common = m0 & m1;

    /* popcount of a 10‑bit mask */
    unsigned int count = 0;
    while (common) {
        common &= common - 1;
        ++count;
    }
    return (seq_t)count;
}
