/*
 * A001557  a(n) = 1^n + 2^n + ... + 10^n.
 * https://oeis.org/A001557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..10} j^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   a1557_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a1557_pow.h"

seq_t a(idx_t n) {
    seq_t result = 0;
    int j;
    for (j = 1; j <= 10; j++) {
        result += a1557_pow((seq_t)j, n);
    }
    return result;
}
