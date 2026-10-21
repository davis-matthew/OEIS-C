/*
 * A061007  a(n) = -(n-1)! mod n.
 * https://oeis.org/A061007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(n-1)! mod n (equals 1 for primes, 0 for composites
 *            except a(4)=2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 4)
        return (seq_t)2;
    if (is_prime(n))
        return (seq_t)1;
    return (seq_t)0;
}
