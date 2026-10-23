/*
 * A129069  Numbers k such that (k-3)/2 is prime.
 * https://oeis.org/A129069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*prime(n) + 3
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);           /* prime(1) = 2 */
    return (seq_t)((seq_t)2 * (seq_t)p + 3);
}
