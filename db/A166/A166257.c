/*
 * A166257  Odd numbers not of the form prime(k) + phi(prime(k)).
 * https://oeis.org/A166257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd integers that are NOT of the form 2p-1 for any prime
 *            p
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t odd = 1;          /* current odd number to test */
    idx_t count = 0;        /* how many included odds we have found */

    for (;;)
    {
        /* A number odd equals 2p-1 iff (odd+1)/2 is prime. */
        idx_t p = (odd + 1) / 2;
        if (!is_prime(p))
        {
            ++count;
            if (count == n)
                return (seq_t)odd;   /* cast to the return type */
        }
        odd += 2;                 /* move to the next odd number */
    }
}
