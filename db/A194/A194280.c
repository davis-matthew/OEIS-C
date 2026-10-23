/*
 * A194280  Inverse permutation to A081344.
 * https://oeis.org/A194280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A194280 – Inverse permutation to A081344
 *
 * Formula:
 *   t = floor(sqrt(n-1)) + 1
 *   A = min(t, n - (t-1)^2)
 *   B = min(t, t^2 - n + 1)
 *   i = (t mod 2) ? A : B
 *   j = (t mod 2) ? B : A
 *   a(n) = (i + j - 1)*(i + j - 2)/2 + j
 *
 * This implementation computes the sequence for any n ≥ 1 using only
 * integer arithmetic.  It defines a small helper for integer square
 * root that works for 64‑bit indices, avoiding the need for external
 * library functions.
 */

#include <stdint.h>

static idx_t isqrt(idx_t n)
{
    /* Integer floor square root by binary search.
       Works for all 64‑bit unsigned values. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << ((sizeof(idx_t) * 8 + 1) / 2); /* ceil of sqrt(UINT64_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            if (mid == hi) break;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (lo == hi && (lo * lo) <= n) ? lo : hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n = 0; sequence starts at n = 1 */

    /* Compute t = floor(sqrt(n-1)) + 1 */
    idx_t t = isqrt(n - 1) + 1;
    idx_t t_minus = t - 1;

    /* Squares of (t-1) and t – safe in 64‑bit for n up to 10⁴ (and more). */
    idx_t sq1 = t_minus * t_minus;
    idx_t sq2 = t * t;

    /* A = min(t, n - (t-1)^2) */
    idx_t diff1 = (n > sq1) ? (n - sq1) : 0;
    idx_t A = (diff1 < t) ? diff1 : t;

    /* B = min(t, t^2 - n + 1) */
    idx_t diff2 = sq2 - n + 1;   /* always ≥ 1 for t defined above */
    idx_t B = (diff2 < t) ? diff2 : t;

    /* Alternate i and j depending on the parity of t */
    idx_t i = (t & 1) ? A : B;
    idx_t j = (t & 1) ? B : A;

    /* Final pairing function calculation.  All intermediate values
       are handled in seq_t as required. */
    seq_t sum = (seq_t)i + (seq_t)j;
    seq_t res = ((seq_t)(sum - 1) * (sum - 2)) / 2 + j;
    return res;
}
