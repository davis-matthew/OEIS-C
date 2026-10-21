/*
 * A211487  Characteristic sequence of numbers n having a primitive root
 *          modulo n.
 * https://oeis.org/A211487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n = 2, 4, p^k, or 2*p^k (p odd prime)
 * Range:     u8 n <= 9594, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   is_odd_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_odd_prime_power.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 4) return (seq_t)1;

    if ((n & 1ULL) == 0) {                     /* even n */
        uint64_t m = n >> 1;                   /* m = n / 2 */
        if ((m & 1ULL) == 0)                   /* m even → not allowed */
            return (seq_t)0;
        return (seq_t)is_odd_prime_power(m);
    } else {                                   /* odd n */
        return (seq_t)is_odd_prime_power(n);
    }
}
