/*
 * A049716  a(n) = 2*n + 1 - prevprime(2*n + 1).
 * https://oeis.org/A049716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 - prevprime(2*n + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    /* Compute m = 2*n + 1 */
    idx_t m = 2 * n + 1;

    /* Find the previous prime < m */
    idx_t pp = prev_prime(m);

    /* Return the difference */
    return (seq_t)(m - pp);
}
