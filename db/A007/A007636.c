/*
 * A007636  Numbers k such that k^2 + k + 17 is composite.
 * https://oeis.org/A007636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th number k such that k^2 + k + 17 is composite
 * Range:     u8 none, u16 n <= 222, u32 n <= 3500, u64 n <= 3500,
 *            u128 n <= 3500, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    
    for (seq_t k = 0; ; k++) {
        seq_t value = k * k + k + 17;
        if (!is_prime((uint64_t)value)) {
            count++;
            if (count == n) {
                return k;
            }
        }
    }
}
