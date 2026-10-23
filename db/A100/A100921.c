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
 * Helpers:   popcnt_fast, zero_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcnt_fast.h"
#include "../utils/zero_bits.h"

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
