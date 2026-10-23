/*
 * A180217  a(n) = (n-th prime modulo 3) + (n-th prime modulo 4).
 * https://oeis.org/A180217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th prime mod 3) + (n-th prime mod 4)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime; the utility returns it as uint32_t. */
    uint32_t p = get_prime(n);

    /* compute remainders modulo 3 and 4, then add them. */
    uint32_t r3 = p % 3;
    uint32_t r4 = p % 4;

    return (seq_t)(r3 + r4);
}
