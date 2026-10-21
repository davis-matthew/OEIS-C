/*
 * A228353  Primes of the form 3*p - 4 where p is prime.
 * https://oeis.org/A228353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 3*A258261(n)-4 where A258261(n) is the nth prime
 * Range:     u8 n <= 15, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n<1, guard only */

    idx_t count = 0;
    idx_t p = 2;            /* start with the smallest prime */

    seq_t q;
    for (;; ++p)
    {
        if (is_prime(p))
        {
            q = (seq_t)3 * p - 4;
            if (is_prime_big(q))
            {
                ++count;
                if (count == n)
                    return q;
            }
        }
    }
}
