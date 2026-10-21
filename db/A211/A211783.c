/*
 * A211783  Rectangular array: R(n,k)=n^2+[(n^2)/2]+...+[(n^2)/k], where [
 *          ]=floor, by antidiagonals.
 * https://oeis.org/A211783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Sequence A211783: flatten the 2‑D array
   R(m,k)=m²+⌊m²/2⌋+⋯+⌊m²/k⌋ by antidiagonals.
   The index n is obtained by:
     * let d be the diagonal number:   d = ⌊(√(8n+1)-1)/2⌋
     * let offset = n - d(d+1)/2  (0 ≤ offset ≤ d)
     * then m = d - offset + 1,  k = offset + 1
   Return Σ_{j=1}^{k} ⌊m²/j⌋. */

static inline idx_t isqrt_u128(unsigned __int128 n)
{
    idx_t lo = 0, hi = (idx_t)-1; /* 2¹⁶⁴-1 for 64‑bit idx_t */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)      lo = mid + 1;
        else             hi = mid - 1;
    }
    return hi; /* floor sqrt */
}

seq_t a(idx_t n)
{
    /* Find diagonal d */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_u128(t);
    idx_t d = (s - 1) / 2;

    /* Elements before this diagonal */
    idx_t t0 = (idx_t)(((unsigned __int128)d * (d + 1)) >> 1);

    idx_t offset = n - t0;

    idx_t m = d - offset + 1; /* first coordinate (square root) */
    idx_t k = offset + 1;     /* second coordinate */

    seq_t sum = 0;
    seq_t m_sq = (seq_t)m * (seq_t)m;
    for (idx_t j = 1; j <= k; ++j)
        sum += m_sq / (seq_t)j;
    return sum;
}
