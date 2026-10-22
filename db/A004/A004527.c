/*
 * A004527  Ratios of successive terms are 1,2,2,3,4,4,5,6,6,...
 * https://oeis.org/A004527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product from i=1 to n of r(i), where r(1)=1 and r(i+1) is
 *            from sequence 2,2,3,4,4,5,6,6,
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 37,
 *            big any n
 * Helpers:   ratio_at_pos_0indexed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ratio_at_pos_0indexed.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        idx_t ratio = (i == 1) ? 1 : ratio_at_pos_0indexed(i - 2);
        result *= (seq_t)ratio;
    }
    return result;
}
