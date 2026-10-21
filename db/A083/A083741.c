/*
 * A083741  a(n) = L(n) + a(L(n)), where L(n) = n - 2^floor(log_2(n))
 *          (A053645).
 * https://oeis.org/A083741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..k} i*2^e[i] where the binary expansion of n is
 *            n = Σ 2^e[i] with decreasing exponents e[0] > … > e[k] (A272011)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    /* Find the position of the most significant 1-bit. */
    idx_t msb_pos = floor_log2_uint64(n);
    seq_t sum = 0;
    /* i counts the index of the current set bit in the
       descending order (e[0] has i = 0). */
    idx_t i = 0;
    /* Iterate from msb_pos down to 0 inclusive. */
    for (idx_t pos = msb_pos; ; --pos) {
        if (n & ((idx_t)1ULL << pos)) {
            sum += (seq_t)i << pos;
            ++i;
        }
        if (pos == 0) break;
    }
    return sum;
}
