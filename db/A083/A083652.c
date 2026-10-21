/*
 * A083652  Sum of lengths of binary expansions of 0 through n.
 * https://oeis.org/A083652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2 + (n+1)*ceil(log_2(n+1)) - 2^ceil(log_2(n+1))
 * Range:     u8 n <= 51, u16 n <= 5670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ceil_log2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ceil_log2_u64.h"

seq_t a(idx_t n) {
    /* Compute r = ceil(log2(n+1)) */
    idx_t t = n + 1;            /* t = n+1, defined up to 2^64-1 */
    unsigned int r = ceil_log2_u64(t);

    /* pow2 = 2^r  (r <= 64, n <= 2^64-1) */
    seq_t pow2 = 1;
    for (unsigned int i = 0; i < r; ++i) {
        pow2 <<= 1;
    }

    /* a = 2 + (n+1)*r - pow2 */
    seq_t ans = 2 + ((seq_t)n + 1) * (seq_t)r - pow2;
    return ans;
}
