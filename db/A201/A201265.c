/*
 * A201265  Primes of the form 9n^3+4.
 * https://oeis.org/A201265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form 9n^3+4 (OEIS A201265)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 69, u64 n <= 3200,
 *            u128 n <= 3200, big any n
 * Validated: standard, pass, 43 terms through n = 2977 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    for (idx_t i = 1; ; ++i) {
        seq_t k = (seq_t)i;
        seq_t v = (k * k * k) * 9 + 4;  /* 9 * i^3 + 4 */
        if (is_prime_seq(v)) {
            ++found;
            if (found == n)
                return v;
        }
    }
}
