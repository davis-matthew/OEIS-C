/*
 * A087276  Write n in binary: 1ab..yz, then a(n) = 1ab..yz + ... + 1yz + 1z +
 *          1.
 * https://oeis.org/A087276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2n)=2*a(n)+1; a(2n+1)=2*a(n)+floor(log2(n))+2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   log2_floor_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/log2_floor_idx.h"

seq_t a(idx_t n) {
    if (n <= 1) {
        return 1;
    }
    if ((n & 1) == 0) {                 /* n = 2k */
        return 2 * a(n >> 1) + 1;
    } else {                             /* n = 2k + 1, k = n >> 1 */
        idx_t k = n >> 1;
        seq_t logk = log2_floor_idx(k);
        return 2 * a(k) + logk + 2;
    }
}
