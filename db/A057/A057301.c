/*
 * A057301  Binary counter with bit positions permuted in increasing length
 *          cycles: 0;2,1;5,3,4;9,6,7,8,...
 * https://oeis.org/A057301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is obtained by permuting the binary * bit positions of n
 *            according to increasing‑length cycles. * Cycle k (k≥1) uses k
 *            positions: * start + (k-1), start + 0, start + 1, …, start +
 *            (k-2) * where start = k(k-1)/2. For each set bit at index i of
 *            n, * the bit is moved to its permuted position po...
 * Range:     u8 n <= 63, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   permuted_position
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/permuted_position.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t  i   = 0;               /* bit index in n (LSB=0) */

    while (n) {
        if (n & (idx_t)1) {
            idx_t pos = permuted_position(i);
            res += ((seq_t)1) << pos;
        }
        n >>= 1;
        ++i;
    }
    return res;
}
