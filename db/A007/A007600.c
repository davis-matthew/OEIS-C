/*
 * A007600  Minimal number of subsets in a separating family for a set of n
 *          elements.
 * https://oeis.org/A007600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Min_{k=0..2} 2*k + 3*ceiling(log_3(n/2^k))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   ceil_log3_divide
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_log3_divide.h"

seq_t a(idx_t n) {
    seq_t r0 = 3 * ceil_log3_divide(n, 1ULL);
    seq_t r1 = 2 + 3 * ceil_log3_divide(n, 2ULL);
    seq_t r2 = 4 + 3 * ceil_log3_divide(n, 4ULL);
    
    if (r1 < r0) r0 = r1;
    if (r2 < r0) r0 = r2;
    
    return r0;
}
