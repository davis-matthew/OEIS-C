/*
 * A179394  a(n) = prime(n)^2 mod prime(n+2).
 * https://oeis.org/A179394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^2 mod prime(n+2)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Return 0 for n < 1; the offset for this sequence is 1. */
    if (n < 1)
        return 0;

    uint32_t p1 = get_prime(n);       /* prime(n)            */
    uint32_t p2 = get_prime(n + 2);   /* prime(n+2)          */

    seq_t sq = (seq_t)p1 * (seq_t)p1;
    return sq % (seq_t)p2;
}
