/*
 * A007915  Duplicate of A004709.
 * https://oeis.org/A007915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   is_cube_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th cube-free positive integer (no prime factor appears with exponent >= 3) */

#include <stdbool.h>
#include "../utils/is_cube_free.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    while (count < (seq_t)n) {
        k++;
        if (is_cube_free(k)) {
            count++;
        }
    }
    return k;
}
