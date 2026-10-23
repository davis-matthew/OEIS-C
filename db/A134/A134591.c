/*
 * A134591  a(n) is n reflected in n-th prime: distance between a(n) and
 *          prime(n) equals distance between prime(n) and n.
 * https://oeis.org/A134591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n) - n
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    uint32_t p = get_prime(n);          /* n-th prime */
    seq_t res = (seq_t)p * 2 - (seq_t)n;/* a(n) = 2*prime(n) - n */
    return res;
}
