/*
 * A103565  Primes of the form 3*p^2+2, where p is prime.
 * https://oeis.org/A103565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 3*(A103564(n))^2 (i.e., 2 + 3 * p_n^2, where p_n is
 *            the nth prime)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t found = 0;
    uint64_t prime = 2;               /* current prime p_n */

    for (;;)
    {
        /* compute candidate value 2 + 3*p^2 */
        unsigned __int128 tmp = (unsigned __int128)3 * prime * prime + 2;
        seq_t candidate = (seq_t)tmp;

        if (is_prime_seq(candidate))
        {
            ++found;
            if (found == n)
                return candidate;
        }

        /* advance to the next prime */
        if (prime == 2)
            prime = 3;
        else
        {
            prime += 2;
            while (!is_prime_seq((seq_t)prime))
                prime += 2;
        }
    }
}
