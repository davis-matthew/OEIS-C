/*
 * A049234  Number of divisors of prime(n) + 2.
 * https://oeis.org/A049234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(prime(n) + 2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of prime(n) + 2. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    uint32_t p = get_prime(n);               /* fetch the nth prime */
    uint64_t val = (uint64_t)p + 2ULL;        /* compute prime(n)+2        */
    return divisor_count_u64(val);            /* number of divisors of val  */
}
