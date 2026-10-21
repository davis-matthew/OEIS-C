/*
 * A075443  a(n)=(1/2)Sum_h |h-h'| with h and h' in [1,n], gcd(h,n)=1, hh'=1
 *          (mod n).
 * https://oeis.org/A075443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{h∈R} |h - h⁻¹ mod n|, where R is the set of
 *            integers 1 ≤ h < n with gcd(h,n)=1
 * Range:     u8 n <= 30, u16 n <= 630, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_idx_t, mod_inverse_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx_t.h"
#include "../utils/mod_inverse_idx_t.h"

/* Compute a(n) for the OEIS A075443. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t h = 1; h < n; ++h) {
        if (gcd_idx_t(h, n) != 1) continue;
        idx_t inv = mod_inverse_idx_t(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }

    return sum / 2;
}
