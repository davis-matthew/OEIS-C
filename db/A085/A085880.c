/*
 * A085880  Triangle T(n,k) read by rows: multiply row n of Pascal's triangle
 *          (A007318) by the n-th Catalan number (A000108).
 * https://oeis.org/A085880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Catalan(r) * binomial(r, k) where r = floor((sqrt(8*n +
 *            1) - 1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 109, u64 n <= 335,
 *            u128 n <= 1148, big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/catalan.h"

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;  /* high enough for 64‑bit n */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq <= n) {
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
    /* compute row number r and offset */
    idx_t t = (n << 3) + 1;             /* 8*n + 1 */
    idx_t s = isqrt_u64(t);             /* floor(sqrt(t)) */
    idx_t r = (s - 1) / 2;              /* largest r with r*(r+1)/2 <= n */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;               /* position within row r */

    seq_t bin = binomial(r, k);         /* C(r,k) */
    seq_t cat = catalan(r);             /* Catalan(r) */

    return bin * cat;                   /* T(r,k) */
}
