/*
 * A088627  Let 2n = r*s. Then a(n) = number of primes of the form r+s (r= 1
 *          and s = 2n contributes 1 to the count if 2n+1 is prime).
 * https://oeis.org/A088627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form r+s with r*s = 2n
 *            (considered for r ≤ s)
 * Range:     u8 n <= 727, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t twoN  = (seq_t)n * 2;          /* 2n */
    idx_t d = 1;
    for (; (seq_t)d * (seq_t)d <= twoN; ++d)
    {
        if (twoN % d == 0)
        {
            idx_t e     = (idx_t)(twoN / d);   /* complementary factor */
            seq_t sum   = (seq_t)d + (seq_t)e; /* r + s */
            if (_is_prime((uint64_t)sum))
                count++;
        }
    }
    return count;
}
