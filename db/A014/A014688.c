/*
 * A014688  a(n) = n-th prime + n.
 * https://oeis.org/A014688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + A000040(n) = n + get_prime(n)
 * Range:     u8 n <= 46, u16 n <= 6018, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for n < 1, return 0 for safety */
    /* a(n) = n + the n-th prime */
    return (seq_t)n + (seq_t)get_prime(n);
}
