/*
 * A075448  A075443(n^2)-A075443(n) mod n [i.e., least nonnegative residue].
 * https://oeis.org/A075448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A075443(n^2) - A075443(n)) mod n, with A075443(m) =
 *            (1/2)*Sum_{1<=h<m, gcd(h,m)=1} |h - h^(-1) mod m|; sums are kept
 *            mod 2n (the full sum is even)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* (1/2)*Sum |h - inv(h)| over units h mod m, reduced mod n (via the sum mod 2n) */
static uint64_t a075448_half_sum_mod(uint64_t m, uint64_t n)
{
    uint64_t md = 2 * n;
    uint64_t s = 0;
    for (uint64_t h = 1; h < m; ++h) {
        int64_t t = 0, nt = 1;
        int64_t r = (int64_t)m, nr = (int64_t)h;
        while (nr != 0) {
            int64_t q = r / nr, tmp;
            tmp = t - q * nt; t = nt; nt = tmp;
            tmp = r - q * nr; r = nr; nr = tmp;
        }
        if (r != 1) continue;
        if (t < 0) t += (int64_t)m;
        uint64_t inv = (uint64_t)t;
        uint64_t diff = (h > inv) ? h - inv : inv - h;
        s = (s + diff % md) % md;
    }
    return s / 2;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t x = a075448_half_sum_mod(nn * nn, nn) % nn;
    uint64_t y = a075448_half_sum_mod(nn, nn) % nn;
    return (seq_t)((x + nn - y) % nn);
}
