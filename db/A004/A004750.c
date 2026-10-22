/*
 * A004750  Numbers whose binary expansion contains the substring '011'.
 * https://oeis.org/A004750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 8133, u32 n <= 8133, u64 n <= 8133,
 *            u128 n <= 8133, big any n
 * Helpers:   contains_011
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_011.h"

/* Search for the n-th positive integer whose binary expansion contains '011' */

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t num = 0;
    while (count < n) {
        num++;
        if (contains_011(num)) {
            count++;
        }
    }
    return num;
}
