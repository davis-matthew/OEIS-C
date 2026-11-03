/*
 * A004741  Concatenation of sequences (1,3,..,2n-1,2n,2n-2,..,2) for n >= 1.
 * https://oeis.org/A004741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Concatenation of blocks where block k = (1, 3, ..., 2k-1, 2k,
 *            2k-2, ..., 2)
 * Range:     u8 n <= 10100, u16 n <= 10100, u32 n <= 10100, u64 n <= 10100,
 *            u128 n <= 10100, big any n
 * Validated: standard, pass, 116 terms through n = 9783 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
        
    // Find which block k the index n falls into
    // Block k has length 2k and ends at position k(k+1)
    idx_t k = isqrt_u64(n);
    while (k * (k + 1) < n) {
        k++;
    }
    
    // Position within block k (1-indexed)
    idx_t pos = n - (k - 1) * k;
    
    if (pos <= k) {
        // Odd numbers part: 1, 3, 5, ..., 2k-1
        return (seq_t)(2 * pos - 1);
    } else {
        // Even numbers part: 2k, 2k-2, ..., 2
        return (seq_t)(2 * (2 * k - pos + 1));
    }
}
