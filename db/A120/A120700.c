/*
 * A120700  a(n) is the least refactorable number k having the n-th prime as
 *          its greatest prime factor.
 * https://oeis.org/A120700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=9, for n>=3: a(n)=8*prime(n)
 * Range:     u8 n <= 11, u16 n <= 1028, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)9;
    uint32_t p = get_prime(n);
    return (seq_t)8 * (seq_t)p;
}
