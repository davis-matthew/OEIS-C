/*
 * A003924  Order of universal Chevalley group B_n (7).
 * https://oeis.org/A003924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n^2) * Product_{k=1}^n (7^(2k) - 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t seven = 7;
    seq_t result = ipow_idx(seven, n * n);
    
    seq_t power_of_seven = 49;  // 7^2
    for (idx_t k = 1; k <= n; k++) {
        result = result * (power_of_seven - 1);
        power_of_seven = power_of_seven * 49;
    }
    
    return result;
}
