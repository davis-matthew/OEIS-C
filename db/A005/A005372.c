/*
 * A005372  a(n) = L(L(n+1)+1), where L(n) are Lucas numbers A000032.
 * https://oeis.org/A005372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = L(L(n+1)+1), where L(n) are Lucas numbers A000032
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n) {
    seq_t l_n_plus_1 = lucas(n + 1);
    idx_t inner_index = (idx_t)(l_n_plus_1 + (seq_t)1);
    return lucas(inner_index);
}
