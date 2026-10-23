/*
 * A175452  a(n) = smallest prime such that a(n)+2 is multiple of 2n+1.
 * https://oeis.org/A175452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 934, u32 n <= 2033, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest prime p such that p + 2 is divisible by (2n + 1) (searched) */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t mod = (seq_t)2 * n + 1;          /* 2n + 1   */
    seq_t cand = (seq_t)2 * n - 1;         /* 2n - 1   */

    if (cand < 2)                         /* ensure cand >= 2                               */
        cand += mod;

    while (!is_prime_big(cand))            /* search the arithmetic progression           */
        cand += mod;

    return cand;
}
