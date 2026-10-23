/*
 * A127920  1/6 of product of three numbers: n-th prime, previous and
 *          following number.
 * https://oeis.org/A127920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n*(p_n-1)*(p_n+1)/6 where p_n is the n-th prime
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 398, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime (1‑based). */
    uint32_t p_small = get_prime(n);
    /* Promote to seq_t and compute the product. */
    seq_t p   = (seq_t)p_small;
    seq_t prod = p * (p - 1) * (p + 1);
    /* Divide by 6; the result is always an integer. */
    return prod / 6;
}
