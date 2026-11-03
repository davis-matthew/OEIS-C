/*
 * A235702  Fixed points of A001175 (Pisano periods).
 * https://oeis.org/A235702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 1; otherwise a(n) = 24*5^(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 1293 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalently, a(1)=1, a(2)=24 and for n>2: a(n)=5*a(n-1).) */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return (seq_t)1;
    }
    seq_t pow5 = ipow_idx((seq_t)5, n - 2);
    return (seq_t)24 * pow5;
}
