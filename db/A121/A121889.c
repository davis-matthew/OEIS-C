/*
 * A121889  Least m such that (prime(n) mod m) > (prime(n+1) mod m).
 * https://oeis.org/A121889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that (prime(n) mod m) > (prime(n+1) mod m)
 * Range:     u8 n <= 53, u16 n <= 8612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);       /* prime(n)   */
    seq_t q = (seq_t)get_prime(n + 1);   /* prime(n+1) */

    for (seq_t m = 2; ; ++m)
    {
        if ((p % m) > (q % m))
            return m;
    }
}
