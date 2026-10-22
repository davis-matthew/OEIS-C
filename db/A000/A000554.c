/*
 * A000554  Number of labeled trees of diameter 3 with n nodes.
 * https://oeis.org/A000554
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(2^(n-3) - 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    seq_t base = (seq_t)n * (seq_t)(n - 1);
    seq_t power = a000272_ipow((seq_t)2, n - 3);
    return base * (power - 1);
}
