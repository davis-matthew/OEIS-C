/*
 * A054075  Position of first appearance of n in A054073.
 * https://oeis.org/A054075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)/2 + #{h in 1..n : frac(h*sqrt(2)) <=
 *            frac(n*sqrt(2))}, i.e. the position of n in the concatenated
 *            permutations p(k) of A054073 (n first appears in p(n))
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>

/* floor(h*sqrt(2)) = floor(sqrt(2*h*h)), computed exactly */
static uint64_t A054075_fl(uint64_t h)
{
    unsigned __int128 v = (unsigned __int128)h * h * 2;
    uint64_t s = (uint64_t)sqrtl((long double)v);
    while ((unsigned __int128)s * s > v) --s;
    while ((unsigned __int128)(s + 1) * (s + 1) <= v) ++s;
    return s;
}

seq_t a(idx_t n)
{
    uint64_t fn = A054075_fl((uint64_t)n);
    uint64_t rank = 1; /* h = n itself */
    for (uint64_t h = 1; h < (uint64_t)n; ++h) {
        uint64_t fh = A054075_fl(h);
        uint64_t d = (uint64_t)n - h;          /* > 0 */
        /* frac(h*s) < frac(n*s)  <=>  -d*sqrt2 < fh - fn */
        bool less;
        if (fh >= fn) {
            less = true;
        } else {
            unsigned __int128 e = fn - fh;
            less = ((unsigned __int128)d * d * 2 > e * e);
        }
        if (less) ++rank;
    }
    seq_t r = (seq_t)n * ((seq_t)n - 1) / 2;
    r += (seq_t)rank;
    return r;
}
