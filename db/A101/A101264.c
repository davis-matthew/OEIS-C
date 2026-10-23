/*
 * A101264  a(n) = 1 if 2*n + 1 is prime, otherwise a(n) = 0.
 * https://oeis.org/A101264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A101264
 *  a(n) = 1 if 2*n+1 is prime, otherwise 0
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    uint64_t p = (uint64_t)n * 2ULL + 1ULL;
    return is_prime(p) ? (seq_t)1 : (seq_t)0;
}
