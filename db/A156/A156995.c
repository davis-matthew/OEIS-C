/*
 * A156995  Triangle T(n, k) = 2*n*binomial(2*n-k, k)*(n-k)!/(2*n-k), with
 *          T(0, 0) = 2, read by rows.
 * https://oeis.org/A156995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the triangle T(r,k) read by rows where T(r,k) =
 *            2*r*binomial(2*r-k,k)*(r-k)!/(2*r-k) for r≥1, T(0,0)=2. The row
 *            r and column k are derived from the linear index n by the
 *            triangular‑number boundaries
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 595, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* integer square root: floor(sqrt(x)) for unsigned 64 bit */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x >> 1;          /* high = floor(x/2) + 1 is safe */
    if (x == 0 || x == 1) return x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t div = (mid != 0) ? x / mid : 0;
        if (mid > div) high = mid - 1;
        else low = mid + 1;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* Special case: the first element of the triangle. */
    if (n == 0) return (seq_t)2;

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t s = isqrt_u64(8*n + 1);
    idx_t r = (s - 1) / 2;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;
    if (r == 0) return (seq_t)2;          /* safety for n>0 but r==0 */

    seq_t rn = (seq_t)r;
    seq_t result = rn + rn;                /* 2*r */

    /* Compute binomial(2*r - k, k) with early cancellation. */
    seq_t bin_num = 1;
    seq_t bin_den = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num_factor = (seq_t)(2*r - k - i + 1);
        seq_t den_factor = (seq_t)i;

        bin_num *= num_factor;
        bin_den *= den_factor;

        seq_t g = gcd(bin_num, bin_den);
        bin_num /= g;
        bin_den /= g;
    }
    /* bin_den should be 1; bin_num is the binomial coefficient. */
    seq_t bin_coeff = bin_num;

    /* Multiply by the binomial coefficient. */
    result *= bin_coeff;

    /* Remaining denominator: (2*r - k).  Reduce before factorial. */
    seq_t den = (seq_t)(2*r - k);
    seq_t g = gcd(result, den);
    result /= g;
    den /= g;

    /* Multiply by (r-k)! with cancellation against any leftover denominator. */
    idx_t factn = r - k;
    for (idx_t j = 1; j <= factn; ++j) {
        g = gcd(result, den);
        result /= g;
        den /= g;

        result *= (seq_t)j;

        g = gcd(result, den);
        result /= g;
        den /= g;
    }

    /* Final division if any denominator remains. */
    g = gcd(result, den);
    result /= g;
    den /= g;
    /* den should be 1 at this point. */

    return result;
}
