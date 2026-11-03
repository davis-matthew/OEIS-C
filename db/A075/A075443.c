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
 * Validated: standard, pass, 81 terms through n = 9837 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_idx.h"

/* Modular inverse of a modulo mod (mod > 1, gcd(a,mod) = 1). */
static idx_t mod_inverse_idx_t(idx_t a, idx_t mod)
{
    int64_t t   = 0;
    int64_t newt = 1;
    int64_t r   = mod;
    int64_t newr = a;

    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp;

        tmp = t - q * newt;
        t   = newt;
        newt = tmp;

        tmp = r - q * newr;
        r   = newr;
        newr = tmp;
    }
    /* inverse exists because gcd(a,mod)=1 */
    if (t < 0) t += mod;
    return (idx_t)t;
}

/* Compute a(n) for the OEIS A075443. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t h = 1; h < n; ++h) {
        if (gcd_idx(h, n) != 1) continue;
        idx_t inv = mod_inverse_idx_t(h, n);
        idx_t diff = (h > inv) ? h - inv : inv - h;
        sum += (seq_t)diff;
    }

    return sum / 2;
}
