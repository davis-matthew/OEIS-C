/*
 * A001226  Lerch's function q_2(n) = (2^{phi(t)} - 1)/t where t = 2*n - 1.
 * https://oeis.org/A001226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^phi(t) - 1)/t with t = 2*n - 1 (computed by bitwise
 *            long division)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a001226_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001226_phi.h"

seq_t a(idx_t n) {
    uint64_t t = 2 * (uint64_t)n - 1;
    uint64_t e = a001226_phi(t);
    uint64_t r = 0, i;
    seq_t q = 0;
    /* long division of the all-ones number 2^e - 1 by t, one bit at a time */
    for (i = 0; i < e; i++) {
        r = 2 * r + 1;
        q = q + q;
        if (r >= t) {
            r -= t;
            q += 1;
        }
    }
    return q;
}
