/*
 * A182200  a(n) = prime(n)^2-3.
 * https://oeis.org/A182200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2-3
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so we expect n >= 1.
       For n == 0 the behaviour is undefined (any value is acceptable). */
    uint32_t p = get_prime(n);        /* nth prime (counting the prime 2 as 1) */
    seq_t v  = (seq_t)p;              /* cast to the result type */
    v = v * v;                        /* square the prime */
    v = v - 3;                        /* subtract 3 */
    return v;
}
