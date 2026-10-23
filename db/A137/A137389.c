/*
 * A137389  a(n) = 2^prime(n) + 2^prime(n+1).
 * https://oeis.org/A137389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^prime(n) + 2^prime(n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; below offset behaviour is irrelevant */
    if (n == 0)
        return 0;

    seq_t t1 = pow2(get_prime(n));      /* 2^prime(n)   */
    seq_t t2 = pow2(get_prime(n + 1));  /* 2^prime(n+1) */
    return t1 + t2;
}
