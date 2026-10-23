/*
 * A136020  Smallest prime of the form (2*n+1)*prime(k)-2*n, any k.
 * https://oeis.org/A136020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that (p+2*n)/(2*n+1) is prime
 * Range:     u8 n <= 14, u16 n <= 517, u32 n <= 3125, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 just in case */

    seq_t d = (seq_t)2 * (seq_t)n + 1;   /* divisor 2*n+1 */
    seq_t c = (seq_t)2 * (seq_t)n;       /* constant 2*n          */

    for (idx_t q = 2; ; ++q)            /* iterate over prime q */
    {
        if (!is_prime_big((seq_t)q))
            continue;                   /* skip composite q */

        seq_t p = (seq_t)q * d - c;      /* candidate prime p   */

        if (p >= 2 && is_prime_big(p))
            return p;                   /* first p is the answer */
    }
}
