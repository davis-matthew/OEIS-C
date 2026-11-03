/*
 * A100921  n appears A023416(n) times (appearances equal number of 0-bits).
 * https://oeis.org/A100921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth term of the multiset that contains each integer
 *            m repeated A023416(m) times, where A023416(m) is the number of
 *            0‑bits in the binary representation of m (with 0 having one
 *            0‑bit). Therefore a(n) is the smallest m such that
 *            sum_{k=0}^{m-1} A023416(k) <= n < sum_{k=0}^{m} A023416(k)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   popcount_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_idx.h"

/* Count the number of 0‑bits in the binary representation of m.
   For m = 0 we return 1 (binary "0"). */
static inline idx_t zero_bits(idx_t m) {
    if (m == 0) return 1;
    idx_t bits = 0;
    idx_t t = m;
    while (t) {
        ++bits;
        t >>= 1;
    }
    return bits - popcount_idx(m);
}

seq_t a(idx_t n) {
    /* Scan successive integers accumulating their zero‑bit counts until the
       cumulative count surpasses the requested index. */
    idx_t target = n;
    idx_t cum = 0;
    for (idx_t m = 0; ; ++m) {
        idx_t z = zero_bits(m);
        if (cum + z > target) {
            return (seq_t)m;
        }
        cum += z;
    }
}
