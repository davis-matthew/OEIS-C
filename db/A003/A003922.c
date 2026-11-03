/*
 * A003922  Order of universal Chevalley group B_n (5).
 * https://oeis.org/A003922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n^2) * Product_{i=1}^{n} (5^(2i) - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t result = ipow_idx(5, n * n);
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t power_of_5 = ipow_idx(5, 2 * i);
        result *= (power_of_5 - 1);
    }
    
    return result;
}
