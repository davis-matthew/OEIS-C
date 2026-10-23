/*
 * A123608  Numbers k such that k, k+1 and 2*k+1 are composite.
 * https://oeis.org/A123608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k, k+1 and 2k+1 are composite. */
#include <stdbool.h>
#include "../utils/is_prime_generic.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        /* k must be composite */
        if (is_prime_generic(k))
            continue;

        /* k+1 must be composite */
        if (is_prime_generic(k + 1))
            continue;

        /* 2k+1 must be composite */
        if (is_prime_generic(2 * k + 1))
            continue;

        ++count;
        if (count == n)
            return (seq_t)k;
    }
}
