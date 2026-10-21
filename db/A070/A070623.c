/*
 * A070623  a(n) = n^5 mod 40.
 * https://oeis.org/A070623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 40
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 40 first – n^5 mod 40 depends only on n mod 40. */
    uint64_t base = n % 40ULL;        /* base ∈ [0,39] */
    uint64_t result = 1;

    /* Compute base^5 mod 40 by repeated multiplication. */
    for (int i = 0; i < 5; ++i)
        result = (result * base) % 40ULL;

    return (seq_t)result;
}
