/*
 * A140777  a(n) = 2*prime(n) - 4.
 * https://oeis.org/A140777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n) - 4 (OEIS A140777)
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint32_t p = get_prime(n);     /* kth prime */
    return (seq_t)p * 2 - 4;
}
