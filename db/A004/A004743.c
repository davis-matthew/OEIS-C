/*
 * A004743  Numbers whose binary expansion does not contain 110.
 * https://oeis.org/A004743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 4180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_110
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_110.h"

/* Search for the n-th non-negative integer whose binary expansion does not contain 110 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    
    while (1) {
        if (!contains_110(num)) {
            count++;
            if (count == (seq_t)n) {
                return num;
            }
        }
        num++;
    }
}
