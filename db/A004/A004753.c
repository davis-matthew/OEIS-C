/*
 * A004753  Numbers whose binary expansion contains 100.
 * https://oeis.org/A004753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for n-th number whose binary expansion contains 100
 * Range:     u8 n <= 168, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_100.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t candidate = 0;
    
    while (count < (seq_t)n) {
        candidate++;
        if (contains_100(candidate)) {
            count++;
        }
    }
    
    return candidate;
}
