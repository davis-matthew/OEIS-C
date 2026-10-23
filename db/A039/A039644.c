/*
 * A039644  Number of steps to fixed point of "k -> k/2 or (k+1)/2 until
 *          result is prime", starting with prime(n)-1.
 * https://oeis.org/A039644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps of k -> k/2 or (k+1)/2 until a prime or
 *            the fixed point 1 is reached, starting from prime(n)-1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* starting value k = prime(n) - 1 */
    uint32_t p = get_prime(n);
    idx_t k = (idx_t)p - 1U;

    /* Special case k == 1 : it is already the fixed point. */
    if (k == 1U)
        return (seq_t)1;

    seq_t steps = 0;
    while (!is_prime(k) && k != 1U) {
        if ((k & 1U) == 0U)       /* even */
            k >>= 1;
        else                      /* odd -> (k+1)/2 */
            k = (k + 1U) >> 1;
        ++steps;
    }

    /* If we stopped at 1, return the number of steps. */
    if (k == 1U)
        return steps;

    /* Otherwise we ended at a prime.  Count the transition that
       produced the prime. */
    return steps + 1U;
}
