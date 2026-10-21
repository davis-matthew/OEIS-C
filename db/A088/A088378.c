/*
 * A088378  a(n) = (smallest prime factor of n)^3; a(1) = 1.
 * https://oeis.org/A088378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (least prime factor of n)³, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the cube of the smallest prime factor of n.
   For n==1 the value is defined as 1. */
seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n <= 1)
        return (seq_t)1;

    /* 2 is the smallest factor for all even n */
    if ((n & 1ULL) == 0ULL)
        return (seq_t)(2ULL * 2ULL * 2ULL);   /* 8 */

    /* Try odd divisors up to √n to find the smallest factor */
    uint64_t p = 0;
    for (uint64_t d = 3; d * d <= (uint64_t)n; d += 2) {
        if ((uint64_t)n % d == 0ULL) {
            p = d;
            break;
        }
    }

    /* If no divisor was found, n itself is prime */
    if (p == 0)
        p = (uint64_t)n;

    /* Compute the cube; it fits in 64 bits for n <= 10000.
       Convert the result to seq_t (truncating if necessary). */
    uint64_t cube = p * p * p;
    return (seq_t)cube;
}
