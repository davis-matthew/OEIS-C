/*
 * A100307  Modulo 2 binomial transform of 3^n.
 * https://oeis.org/A100307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n, k) mod 2) * 3^k Equivalent:
 *            a(n) = ∏_{i : bit i of n is 1} (1 + 3^{2^i})
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 188 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* lazy precomputation of 3^{2^i} for used bit positions */
    static seq_t pow3[64];
    static bool need[64] = {false};

    seq_t r = (seq_t)1;
    idx_t m = n;
    int i = 0;
    while (m) {
        if (m & 1) {
            if (!need[i]) {
                /* exponent is 2^i, i.e. 1ULL << i */
                pow3[i] = ipow_idx((seq_t)3,
                                  (idx_t)((idx_t)1 << i));
                need[i] = true;
            }
            r *= ((seq_t)1 + pow3[i]);
        }
        m >>= 1;
        ++i;
    }
    return r;
}
