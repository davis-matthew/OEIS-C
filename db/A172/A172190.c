/*
 * A172190  a(n) = 2*prime(n)^3.
 * https://oeis.org/A172190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n)^3
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 209, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1-based index */
    uint32_t p_uint32 = get_prime(n);
    seq_t p = (seq_t)p_uint32;
    /* compute 2*p^3 */
    seq_t res = 2 * p * p * p;
    return res;
}
