/*
 * A122176  Triangle, read by rows, where T(n,k) = C( k*(k+1)/2 + n-k + 1,
 *          n-k) for n>=k>=0.
 * https://oeis.org/A122176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(Triangle) where T(n,k) = C( k*(k+1)/2 + n-k + 1,
 *            n-k ) for n >= k >= 0 (e.g. read the triangle by rows as given
 *            in OEIS A122176)
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 178, u64 n <= 477,
 *            u128 n <= 1344, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Multiply‑and‑divide binomial implementation that never overflows
   the intermediate results.  It uses only seq_t arithmetic and
   the gcd routine. */
static seq_t binomial_custom(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);  /* numerator term */
        seq_t den = (seq_t)i;             /* denominator term */

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;

        result *= num;
        /* den should now be 1 and can be discarded */
    }
    return result;
}

/* Main sequence accessor.  Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    /* Column k within the row. */
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* Binomial parameters: a = k*(k+1)/2 + (r-k) + 1, b = r-k. */
    unsigned __int128 part1 = (unsigned __int128)k * (k + 1) / 2;
    idx_t a_idx = (idx_t)(part1 + (r - k) + 1);
    idx_t b_idx = r - k;

    return binomial_custom(a_idx, b_idx);
}
