/*
 * A001556  a(n) = 1^n + 2^n + ... + 9^n.
 * https://oeis.org/A001556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=1..9} j^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    for (int j = 1; j <= 9; j++) {
        result += a000272_ipow((seq_t)j, n);
    }
    return result;
}
