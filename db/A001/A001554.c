/*
 * A001554  a(n) = 1^n + 2^n + ... + 7^n.
 * https://oeis.org/A001554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..7} j^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   a001554_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001554_ipow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    int j;
    for (j = 1; j <= 7; j++) {
        sum += a001554_ipow((seq_t)j, n);
    }
    return sum;
}
