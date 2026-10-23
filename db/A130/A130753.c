/*
 * A130753  A folded-back triangular sequence based on symmetry of CnH2*n+2
 *          straight chain alkanes and the number of hydrogen atoms of a given
 *          symmetry type: Besides methane at 4 there are only three symmetry
 *          types: two CH3's->6: a single CH2->2, two CH2's->4.
 * https://oeis.org/A130753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by unfolding the triangular array defined by
 *            a(n,m) = 4 except: – if n==1 && m>1 then 6, – if m is odd and
 *            n==floor(m/2)+1 then 2. The sequence a(k) is produced by
 *            iterating columns m=1,2,… and listing rows n=1…ceil(m/2) for
 *            each column
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned 64 bit integer */
static inline idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = 0xFFFFFFFFULL;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* cumulative counts up to column m: f(m) = Σ_{i=1}^m ceil(i/2) */
static inline idx_t cum(idx_t m) {
    if ((m & 1) == 0) {               /* m even: m = 2M */
        idx_t M = m >> 1;
        return M * (M + 1);           /* M*(M+1) */
    } else {                          /* m odd:  m = 2M+1 */
        idx_t M = (m + 1) >> 1;
        return M * M;                 /* (M)^2 */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;       /* unused, offset is 1 */

    idx_t k = n;

    /* find column m such that cum(m‑1) < k ≤ cum(m) */
    idx_t sqrt_k = isqrt_u64(k);
    idx_t lo = 1;
    idx_t hi = sqrt_k * 2 + 4;          /* safe upper bound */

    while (cum(hi) < k) hi <<= 1;       /* only for extremely large n */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum(mid) >= k) hi = mid;
        else lo = mid + 1;
    }
    idx_t m = lo;
    idx_t prev = cum(m - 1);
    idx_t t = k - prev;                 /* position within this column (1‑based) */

    /* determine the value from the rule */
    if (t == 1 && m == 1) return (seq_t)4;
    if (t == 1 && m > 1)  return (seq_t)6;
    if ((m & 1) && t == (m >> 1) + 1) return (seq_t)2;
    return (seq_t)4;
}
