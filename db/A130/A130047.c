/*
 * A130047  Left half of Pascal's triangle (A034868) modulo 2.
 * https://oeis.org/A130047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r,k) mod 2, where (r,k) are obtained by *
 *            flattening the left half of Pascal's triangle (k ≤ ⌊r/2⌋). The
 *            row * r is the smallest integer such that the cumulative number
 *            of entries * up to row r exceeds n. Then k = n - C(r-1), where
 *            C(r) is the * total number of entries up to row r...
 * Range:     u8 n <= 2549, u16 n <= 2549, u32 n <= 2549, u64 n <= 2549,
 *            u128 n <= 2549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find m = ⌊√n⌋.  Then:
     *   if n ≥ m(m+1)  → row r = 2m (even), C_prev = m(m+1)
     *   else           → row r = 2m-1 (odd), C_prev = m²
     */
    idx_t m = floor_sqrt_u64(n);
    idx_t r;
    unsigned __int128 C_prev;

    if ((unsigned __int128)n >= (unsigned __int128)m * (m + 1)) {
        r = 2 * m;                    /* even row */
        C_prev = (unsigned __int128)m * (m + 1);
    } else {
        r = 2 * m - 1;                /* odd row (m ≥ 1 except n=0 case handled above) */
        C_prev = (unsigned __int128)m * m;
    }

    idx_t k = n - (idx_t)C_prev;      /* 0 ≤ k ≤ ⌊r/2⌋ */

    /* Lucas theorem: binomial(r,k) mod 2 = 1 iff (k & ~r) == 0. */
    return (seq_t)((k & ~r) == 0);
}
