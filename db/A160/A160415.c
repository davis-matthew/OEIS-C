/*
 * A160415  First differences of A160118.
 * https://oeis.org/A160415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For odd n > 1: let m = (n-1)/2. a(n) = 4 *
 *            3^(popcount(m)-1). For even n: let m = n/2, t = v2(m) (number of
 *            trailing zeros in m), r = popcount(m). if t == 0: if r == 1: f =
 *            2 else: f = 7 * 3^(r-2) else: f = 7 * 3^(r + t - 2) a(n) = 4 * f
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 3^k as seq_t.  k is small (≤ 30). */
static seq_t pow3_uint(unsigned k) {
    seq_t res = (seq_t)1;
    for (unsigned i = 0; i < k; ++i) {
        res *= (seq_t)3;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    if ((n & (idx_t)1) != 0) {          /* odd n > 1 */
        idx_t m = (n - (idx_t)1) >> 1;
        unsigned pop = __builtin_popcountll(m);
        seq_t pow = pow3_uint(pop - 1);   /* pop‑1 ≥ 0 because n>1 */
        return (seq_t)4 * pow;
    } else {                              /* even n */
        idx_t m = n >> 1;
        unsigned tz = 0;
        idx_t tmp = m;
        while ((tmp & (idx_t)1) == 0) {   /* count trailing zeros */
            ++tz;
            tmp >>= 1;
        }
        unsigned r = __builtin_popcountll(m);
        seq_t f;
        if (tz == 0) {
            if (r == 1) {
                f = (seq_t)2;
            } else {
                f = (seq_t)7 * pow3_uint(r - 2);
            }
        } else {
            f = (seq_t)7 * pow3_uint(r + tz - 2);
        }
        return (seq_t)4 * f;
    }
}
