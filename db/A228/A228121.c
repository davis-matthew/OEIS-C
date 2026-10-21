/*
 * A228121  Numbers n such that 3n - 4 is prime.
 * https://oeis.org/A228121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_prime_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228121: Numbers n such that 3*n - 4 is prime. */

#include <stdbool.h>
#include "../utils/_is_prime_idx.h"

seq_t a(idx_t n) {
    idx_t m = 2;
    idx_t found = 0;

    while (1) {
        idx_t p = (idx_t)3 * m - 4;   /* check if 3*m - 4 is prime */
        if (_is_prime_idx(p)) {
            found++;
            if (found == n) return (seq_t)m;
        }
        m++;
    }
}
