/*
 * A004742  Numbers whose binary expansion does not contain 101.
 * https://oeis.org/A004742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_101
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: counts numbers whose binary representation does not contain "101" */

#include <stdbool.h>
#include "../utils/contains_101.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    
    while (1) {
        if (!contains_101(k)) {
            count++;
            if (count == n) {
                return k;
            }
        }
        k++;
    }
}
