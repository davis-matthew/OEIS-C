/*
 * A214721  Least m>0 such that 2*n+1+m and n-m are not relatively prime.
 * https://oeis.org/A214721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{ n mod p | p is a prime divisor of 3*n + 1 }
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t s = 3 * (seq_t)n + 1;          /* s = 3*n + 1 */
    seq_t best = s;                      /* initialize to a large value */
    seq_t temp = s;
    seq_t rn = (seq_t)n;                 /* convert n once for mod operations */

    for (seq_t d = 2; d <= temp / d; ++d)
    {
        if (temp % d == 0)
        {
            /* d is a prime divisor of s */
            seq_t r = rn % d;
            if (r < best)
                best = r;

            /* remove all powers of d from temp */
            while (temp % d == 0)
                temp /= d;
        }
    }

    /* temp now holds the remaining prime factor, if any */
    if (temp > 1)
    {
        seq_t r = rn % temp;
        if (r < best)
            best = r;
    }

    return best;
}
