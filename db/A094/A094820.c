/*
 * A094820  Partial sums of A038548.
 * https://oeis.org/A094820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{c=1}^{⌊√n⌋} ⌊n/c⌋ – ⌊√n⌋(⌊√n⌋–1)/2
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit numbers (floor of √n).  */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1, but safe guard */
    idx_t m = isqrt_idx(n);               /* m = ⌊√n⌋ */
    seq_t sum = 0;

    for (idx_t c = 1; c <= m; ++c) {
        sum += (seq_t)(n / c);            /* ⌊n/c⌋ */
    }

    seq_t sub = (seq_t)m * ((seq_t)m - 1) / 2;  /* m(m–1)/2 */
    return sum - sub;
}
