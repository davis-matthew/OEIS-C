/*
 * A047072  Array A read by diagonals: A(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and no step touches the line y=x unless x=0
 *          or x=h.
 * https://oeis.org/A047072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 259, u32 n <= 793, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   _tri128, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047072: A(h,k) read by antidiagonals
 *
 * For each n≥0 let s be the unique integer such that
 *      s*(s+1)/2 ≤ n < (s+1)*(s+2)/2
 * then
 *      h = n – s*(s+1)/2
 *      k = s – h
 *
 * A(h,k) is the number of paths from (0,0) to (h,k) with unit up/right steps
 * that do not touch the line y=x for 0<x<h.
 *
 *  A(0,0) = 1
 *  A(h,h) = 2·Catalan(h‑1)        (h > 0)
 *  otherwise
 *      A(h,k) = |h‑k|/(h+k) · binom(h+k, min(h,k))
 *
 * The function below implements this definition exactly.  All arithmetic is
 * performed on 'seq_t'; intermediate growth is handled by the arbitrary‑width
 * build; the 64‑bit build remains correct as long as the result fits.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/_tri128.h"

/* Univariate binomial coefficient with safe multiplicative steps */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    return r;
}

/* Catalan number C_m = binom(2m,m)/(m+1) */
static seq_t _catalan(idx_t m)
{
    if (m == 0) return 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i)
        res = res * (seq_t)(m + i) / (seq_t)i;   /* exact at each step */

    seq_t den = (seq_t)(m + 1);
    seq_t g   = gcd(res, den);
    res /= g;
    den /= g;                                   /* den should now be 1 */
    return res / den;
}

seq_t a(idx_t n)
{
    /* Find s with (s)*(s+1)/2 ≤ n < (s+1)*(s+2)/2  */
    idx_t low  = 0;
    idx_t high = 1;
    while (_tri128(high) <= n) high <<= 1;       /* grow high until above n */
    idx_t s = high;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (_tri128(mid) <= n) {
            low = mid + 1;
        } else {
            s = mid;
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* start index of diagonal s */
    unsigned __int128 prod_start = (unsigned __int128)s * (s + 1);
    idx_t start = (idx_t)(prod_start / 2);

    idx_t h = n - start;
    idx_t k = s - h;

    if (h == 0 && k == 0) return 1;              /* (0,0) */
    if (h == k) return _catalan(h - 1) * ((seq_t)2);

    idx_t diff = (h > k) ? (h - k) : (k - h);
    idx_t sum  = h + k;
    idx_t min_hk = (h < k) ? h : k;
    seq_t bin = _binom(h + k, min_hk);

    /* Reduce the fraction diff * bin / sum */
    seq_t num = diff;
    seq_t den = sum;
    seq_t g1  = gcd(num, den);
    num /= g1;
    den /= g1;
    seq_t g2  = gcd(bin, den);
    bin /= g2;
    den /= g2;                                     /* den should be 1 */

    /* Final result */
    return (den == 1) ? num * bin : (num * bin) / den;
}
