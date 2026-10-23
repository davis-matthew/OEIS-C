/*
 * A162153  Differences between the sum of consecutive composites and the
 *          prime that precedes them.
 * https://oeis.org/A162153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n+2)^2 - prime(n+1)^2 - prime(n+2) -
 *            3*prime(n+1))/2
 * Range:     u8 n <= 9, u16 n <= 427, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is first term */
    if (n == (idx_t)0) return 0;   /* undefined for n=0, handled gracefully */

    uint32_t p1 = get_prime(n + 1);   /* prime(n+1) */
    uint32_t p2 = get_prime(n + 2);   /* prime(n+2) */

    seq_t a1 = (seq_t)p1;
    seq_t a2 = (seq_t)p2;

    seq_t num = a2 * a2 - a1 * a1 - a2 - 3 * a1;
    return num / 2;
}
