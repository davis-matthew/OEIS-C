/*
 * A023503  Greatest prime divisor of prime(n) - 1.
 * https://oeis.org/A023503
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime divisor of prime(n) - 1
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    uint32_t p = get_prime(n);          /* prime(n), fits in 32 bits for n up to ~10000 */
    uint64_t m = (uint64_t)p - 1;        /* subtract 1 */
    uint64_t g = gpf(m);                 /* greatest prime divisor of m */
    return (seq_t)g;
}
