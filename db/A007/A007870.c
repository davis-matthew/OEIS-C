/*
 * A007870  Determinant of character table of symmetric group S_n.
 * https://oeis.org/A007870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Product of all parts of all partitions of n
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   generate_partitions_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/generate_partitions_helper.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 1;
    generate_partitions_helper(n, n, 1, &result);
    return result;
}
