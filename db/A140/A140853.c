/*
 * A140853  a(n) = prime(prime(n) - 1) - 1, where prime(n) is the n-th prime.
 * https://oeis.org/A140853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(prime(n) - 1) - 1
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* offset starts at 1, unused value */

    /* first prime index: prime(n) */
    uint32_t p = get_prime(n);          /* p = prime(n) */

    /* second prime index: prime(n) - 1 */
    idx_t idx2 = (idx_t)p - 1U;          /* careful to stay unsigned */

    /* second prime value: prime(prime(n) - 1) */
    uint32_t q = get_prime(idx2);

    /* final result */
    return (seq_t)(q - 1U);
}
