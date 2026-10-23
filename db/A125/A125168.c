/*
 * A125168  a(n) = gcd(n, A032741(n)) where A032741(n) is the number of proper
 *          divisors of n.
 * https://oeis.org/A125168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(n, A032741(n)) -- number of proper divisors of n
 * Range:     u8 n <= 1077, u16 n <= 9811, u32 n <= 9811, u64 n <= 9811,
 *            u128 n <= 9811, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = gcd(n, number of proper divisors of n) */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case n = 1: A032741(1) = 0, gcd(1,0) = 1 */
    if (n == 1)
        return 1;

    idx_t temp = n;
    seq_t divisor_count = 1;          /* total divisor count including n itself */

    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            unsigned exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            divisor_count *= (seq_t)(exp + 1);
        }
    }

    /* Remaining prime factor greater than sqrt(original n) */
    if (temp > 1)
        divisor_count *= 2; /* exponent 1 -> (1+1) */

    seq_t proper_divisors = divisor_count - 1;  /* exclude n itself */
    return gcd((seq_t)n, proper_divisors);
}
