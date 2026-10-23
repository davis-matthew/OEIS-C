/*
 * A107750  If n=0 then 0, else smallest number greater than its predecessor
 *          and having either more or fewer zeros in its binary
 *          representation.
 * https://oeis.org/A107750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = the smallest integer m > a(n-1)
 *            whose binary representation has a different number of zeros than
 *            a(n-1)
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   zeros_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/zeros_count.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t val = 0;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t zerosPrev = zeros_count(val);
        do {
            val = (seq_t)(val + 1);
        } while (zeros_count(val) == zerosPrev);
    }
    return val;
}
