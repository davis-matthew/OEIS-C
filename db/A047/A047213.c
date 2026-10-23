/*
 * A047213  Largest 4th power modulo n.
 * https://oeis.org/A047213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{0 ≤ k < n} (k^4 mod n)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow4_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow4_mod.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 (the offset is 1 as per OEIS) */
    seq_t mod = (seq_t)n;
    seq_t max_val = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t r = pow4_mod((seq_t)k, mod);
        if (r > max_val) {
            max_val = r;
        }
    }
    return max_val;
}
