/*
 * A146967  Triangle read by rows: row n is the expansion of p(x,n)=If[n == 0,
 *          1, (x + 1)^n + 2^(n - 3)*Sum[(2^(m - 1) +n*m - n + 1)*x^m*(1 +
 *          x^(n - 2*m)), {m, 1, n - 1}].
 * https://oeis.org/A146967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) is the (r,c)-entry of the triangle T where * r =
 *            floor((sqrt(8·n + 1) - 1)/2), c = n - r·(r+1)/2. *
 *            T(r,0)=T(r,r)=1; T(0,0)=1, T(1,0)=T(1,1)=1, T(2,1)=4. * For r ≥
 *            3 and 0 < c < r: * T(r,c) = binomial(r,c) + * 2^(r-3) · (
 *            2^(c-1) + 2^(r-c-1) + r² – 2r + 2 )
 * Range:     u8 n <= 21, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = 1ull << 32;  /* sqrt(2^64) < 2^32 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* compute row r */
    idx_t sq = isqrt_u64((idx_t)((unsigned __int128)8 * n + 1));
    idx_t r = (sq - 1) / 2;

    /* start index of row r */
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - start;

    if (c == 0u || c == r) return (seq_t)1;
    if (r == 0u || r == 1u) return (seq_t)1;
    if (r == 2u) return (c == 1u ? (seq_t)4 : (seq_t)1);

    /* r >= 3 and 0 < c < r */
    seq_t comb   = binomial((idx_t)r, (idx_t)c);
    seq_t factor = pow2((idx_t)(r - 3));
    seq_t part1  = pow2((idx_t)(c - 1));
    seq_t part2  = pow2((idx_t)(r - c - 1));
    seq_t rseq   = (seq_t)r;
    seq_t inside = part1 + part2 + rseq * rseq - 2 * rseq + 2;
    seq_t res    = comb + factor * inside;
    return res;
}
