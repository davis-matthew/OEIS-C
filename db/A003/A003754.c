/*
 * A003754  Numbers with no adjacent 0's in binary expansion.
 * https://oeis.org/A003754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 8694, u64 n <= 8694,
 *            u128 n <= 8694, big any n
 * Helpers:   has_no_adjacent_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_no_adjacent_zeros.h"

/* Brute-force enumeration: a(n) is the n-th number with no adjacent 0's in binary */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 0;
    while (1) {
        if (has_no_adjacent_zeros(k)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
