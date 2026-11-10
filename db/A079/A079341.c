/*
 * A079341  Triangular array read by rows: GCDs of entries in A010940.
 * https://oeis.org/A079341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, tabl
 * Formula:   T(i,j) = gcd(C(24,i), C(24,j)) for 0 <= i <= j <= 12, read by
 *            rows i (row i lists j = i..12); a(n) is the n-th entry, 91 terms
 *            in all
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: full, pass, 91 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A079341_binom24(uint64_t n)
{
    if (n > 24) return 0;
    uint64_t k = n;
    if (k > 12) k = 24 - k;
    uint64_t res = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        res = res * (24 - k + i) / i;
    }
    return res;
}

static uint64_t A079341_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t i = 0;
    /* row i has 13 - i entries (j = i..12) */
    while (i <= 12 && m >= 13 - i) {
        m -= 13 - i;
        ++i;
    }
    if (i > 12) return (seq_t)0; /* beyond the finite triangle */
    uint64_t j = i + m;
    return (seq_t)A079341_gcd(A079341_binom24(i), A079341_binom24(j));
}
