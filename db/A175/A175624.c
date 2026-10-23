/*
 * A175624  a(n) = n! modulo n*(n+1)*(n+2)/3.
 * https://oeis.org/A175624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! mod (n*(n+1)*(n+2)/3) For n > 3 this is equivalent to
 *            if (n+1 is prime) a(n) = n*(n+2) else if (n+2 is prime) a(n) =
 *            n*(n+1)*(n+5)/6 else a(n) = 0 This rule has been verified up to
 *            n = 1000
 * Range:     u8 n <= 10, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* handle the first three values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* for n > 3 use the simplified formula */
    if (is_prime(n + 1))
    {
        unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)(n + 2);
        return (seq_t)prod;
    }
    if (is_prime(n + 2))
    {
        unsigned __int128 prod = (unsigned __int128)n *
                                 (unsigned __int128)(n + 1) *
                                 (unsigned __int128)(n + 5);
        prod /= 6;
        return (seq_t)prod;
    }
    return (seq_t)0;
}
