/*
 * A074639  a(n)=Sum_h (hh'-1)/n with h and h' in [1,n], (h,n)=1, hh'=1 (mod
 *          n).
 * https://oeis.org/A074639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{h : (h,n)=1} (h·h' – 1)/n, where h' is the unique
 *            element with (h·h' ≡ 1 (mod n))
 * Range:     u8 n <= 16, u16 n <= 275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_inv.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(0)=a(1)=0 */

    seq_t sum = (seq_t)0;
    for (idx_t h = 1; h < n; ++h) {
        /* Check coprimality */
        idx_t a = h, b = n;
        while (b != 0) { idx_t t = a % b; a = b; b = t; }
        if (a != 1) continue;          /* not coprime */

        idx_t inv = mod_inv(h, n);      /* 0 ≤ inv < n */
        seq_t prod = (seq_t)h * (seq_t)inv;
        seq_t k    = (prod - (seq_t)1) / (seq_t)n;  /* exact division */
        sum += k;
    }
    return sum;
}
