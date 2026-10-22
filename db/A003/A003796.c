/*
 * A003796  Numbers with no 3 adjacent 0's in binary expansion.
 * https://oeis.org/A003796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers with no 3 adjacent 0's in binary expansion. Search
 *            through integers and count those without "000" in binary
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_three_adjacent_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/has_three_adjacent_zeros.h"

seq_t a(idx_t n) {
    seq_t k = 0;
    idx_t count = 0;
    
    while (true) {
        if (!has_three_adjacent_zeros(k)) {
            count++;
            if (count == n) return k;
        }
        k++;
    }
}
