/*
 * A031167  a(n) = prime(n+5) - prime(n).
 * https://oeis.org/A031167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+5) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to prime(6) - prime(1) = 13 - 2 = 11 */
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    uint32_t p1 = get_prime(n);      /* prime(n)   */
    uint32_t p2 = get_prime(n + 5);  /* prime(n+5) */
    return (seq_t)(p2 - p1);         /* difference */
}
