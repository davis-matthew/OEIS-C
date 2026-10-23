/*
 * A038315  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*1^j.
 * https://oeis.org/A038315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, k) * 11^(r-k), where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, floor_isqrt_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_isqrt_u64.h"
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t t = 8ULL * n + 1ULL;
    idx_t s = floor_isqrt_u64(t);
    idx_t r = (s - 1ULL) / 2ULL;

    /* adjust in case the square‑root rounding yielded the wrong r */
    idx_t base = r * (r + 1ULL) / 2ULL;
    if (base > n) {
        --r;
        base = r * (r + 1ULL) / 2ULL;
    }

    idx_t k   = n - base;          /* column index in the row */
    idx_t exp = r - k;             /* exponent for 11 */

    seq_t bin = binomial(r, k);
    seq_t pow = pow_uint(11, exp);
    return bin * pow;
}
