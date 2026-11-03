/*
 * A003832  Order of universal Chevalley group D_n (5).
 * https://oeis.org/A003832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n(n-1)) * (5^n - 1) * ∏_{i=1}^{n-1} (5^(2i) - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t five = (seq_t)5;
    
    result *= ipow_idx(five, n * (n - 1));
    result *= (ipow_idx(five, n) - 1);
    
    for (idx_t i = 1; i < n; i++) {
        result *= (ipow_idx(five, 2 * i) - 1);
    }
    
    return result;
}
