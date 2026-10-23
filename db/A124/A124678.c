/*
 * A124678  Number of conjugacy classes in PSL_2(p), p = prime(n).
 * https://oeis.org/A124678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 for n = 1; a(n) = (prime(n) + 5)/2 for n > 1
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset, return 0 */
    if (n == 1) return (seq_t)3;     /* first term of the sequence */
    uint32_t p = get_prime(n);       /* n‑th prime (1‑based) */
    uint64_t tmp = (uint64_t)p + 5ULL;/* use 64‑bit for intermediate */
    tmp /= 2ULL;
    return (seq_t)tmp;               /* cast to the return type */
}
