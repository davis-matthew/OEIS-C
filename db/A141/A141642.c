/*
 * A141642  Composite numbers whose sum of digits is a prime.
 * https://oeis.org/A141642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th composite integer whose digit sum is a prime
 *            number
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)4;          /* first composite number */
    idx_t count = 0;

    while (1)
    {
        /* Check composite */
        if (x > (seq_t)1 && !is_prime((idx_t)x))
        {
            seq_t dsum = digit_sum(x);
            /* Test if digit sum is prime */
            if (is_prime((idx_t)dsum))
            {
                ++count;
                if (count == n)
                    return x;
            }
        }
        ++x;                     /* Next integer */
    }
}
