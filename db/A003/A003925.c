/*
 * A003925  Order of universal Chevalley group B_n (8).
 * https://oeis.org/A003925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n^2) * Product_{i=1}^{n} (8^(2i) - 1)
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
    // Compute 8^(n^2)
    idx_t n_sq = n * n;
    seq_t result = ipow_idx((seq_t)8, n_sq);
    
    // Compute Product_{i=1}^{n} (8^(2i) - 1)
    for (idx_t i = 1; i <= n; i++) {
        seq_t power = ipow_idx((seq_t)8, 2 * i);
        result *= (power - 1);
    }
    
    return result;
}
