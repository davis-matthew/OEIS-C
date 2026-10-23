/*
 * A201715  Primes of the form 3*m^2 - 2.
 * https://oeis.org/A201715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime of the form 3*m^2 - 2
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t count = 0;
    idx_t m = 5;
    while (1) {
        uint64_t val = 3ull * (uint64_t)m * (uint64_t)m - 2ull;
        if (is_prime_uint64(val)) {
            count++;
            if (count == n) {
                return (seq_t)val;
            }
        }
        ++m;
    }
}
