/*
 * A005558  a(n) is the number of n-step walks on square lattice such that 0
 *          <= y <= x at each step.
 * https://oeis.org/A005558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C(n+1, ceiling(n/2))*C(n, floor(n/2)) - C(n+1,
 *            ceiling((n-1)/2))*C(n, floor((n-1)/2))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 930 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    seq_t term1 = binomial(n + 1, (n + 1) / 2) * binomial(n, n / 2);
    seq_t term2 = binomial(n + 1, n / 2) * binomial(n, (n - 1) / 2);
    
    return term1 - term2;
}
