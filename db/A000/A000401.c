/*
 * A000401  Numbers of form x^2 + y^2 + 2*z^2.
 * https://oeis.org/A000401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sieve/search: a(n) = n-th nonnegative integer not of the form
 *            4^k*(16*m + 14) [Dickson]
 * Range:     u8 n <= 235, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_representable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_representable.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    while (1) {
        if (is_representable(k)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k++;
    }
}
