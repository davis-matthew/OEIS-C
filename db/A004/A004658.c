/*
 * A004658  Powers of 3 written in base 4.
 * https://oeis.org/A004658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-4 representation of 3^n, interpreted as decimal
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 960 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t power = ipow_idx(3, n);
    
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power > 0) {
        seq_t digit = power % 4;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        power = power / 4;
    }
    
    return result;
}
