/*
 * A007890  Summarize the previous term! (in decreasing order).
 * https://oeis.org/A007890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; a(n) = A244112(a(n-1)) for n > 1, i.e. describe the
 *            digits of a(n-1) in decreasing digit order (count followed by
 *            digit); a(n) = 14233221 for n >= 13
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   describe_decreasing
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/describe_decreasing.h"

seq_t a(idx_t n) {
    seq_t current = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t next = describe_decreasing(current);
        if (next == current) break;
        current = next;
    }
    return current;
}
