/*
 * A007638  Numbers k such that 3*k^2 - 3*k + 23 is composite.
 * https://oeis.org/A007638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search for the n-th k where 3*k^2 - 3*k + 23 is composite
 * Range:     u8 none, u16 n <= 53, u32 n <= 3100, u64 n <= 3100,
 *            u128 n <= 3100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    while (count < n) {
        k++;
        seq_t value = 3 * k * k - 3 * k + 23;
        if (!is_prime((uint64_t)value)) {
            count++;
        }
    }
    return k;
}
