/*
 * A221913  Array of coefficients of numerator polynomials (divided by x) of
 *          the n-th approximation of the continued fraction
 *          x/(1+x/(2+x/(3+...
 * https://oeis.org/A221913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = coefficient a(r,c) of the numerators of the convergents
 *            of x/(1+x/(2+x/(3+...)), flattened row‑wise. For r ≥ 1, 0 ≤ c ≤
 *            ⌊(r-1)/2⌋: a(r,c) = binomial(r-1-c, c) * (r-c)!/(c+1)!
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes seq_t and idx_t are defined by the build:
   idx_t  – unsigned 64‑bit integer (uint64_t)
   seq_t  – unsigned integer of arbitrary width (uint8_t … uint64_t or arbitrary precision) */

#include <stdint.h>

/* integer square root, floor of sqrt for unsigned 64‑bit */
static idx_t floor_sqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for zero index, return 0 */

    /* Find the row r containing the n‑th entry (1‑based). */
    idx_t k = floor_sqrt_idx(n);   /* floor(√n)  */
    idx_t r;
    if (n <= k * k) {
        r = 2 * k - 1;             /* odd row  r = 2k-1  */
    } else if (n <= k * (k + 1)) {
        r = 2 * k;                 /* even row r = 2k   */
    } else {
        r = 2 * k + 1;             /* odd row  r = 2k+1  */
    }

    /* Index c (0‑based) of the element inside the row. */
    idx_t prevS;                   /* total entries before row r */
    if ((r & 1) == 0) {            /* even row : r = 2*kr   */
        idx_t kr = r >> 1;         /* kr = r/2                */
        prevS = kr * kr;           /* S_{2k-1} = k^2          */
    } else {                       /* odd  row : r = 2*kr-1   */
        idx_t kr = (r + 1) >> 1;   /* kr = (r+1)/2            */
        prevS = (kr - 1) * kr;     /* S_{2k-2} = (k-1)*k      */
    }
    idx_t c = n - prevS - 1;       /* column position in row  */

    /* Compute a(r,c) = binomial(r-1-c, c) * (r-c)!/(c+1)! */
    idx_t choose_n = r - 1 - c;      /* N = r-1-c */
    idx_t choose_k = c;              /* K = c     */

    /* Binomial coefficient */
    seq_t binom = 1;
    for (idx_t i = 1; i <= choose_k; ++i) {
        seq_t factor = (seq_t)(choose_n - choose_k + i);   /* (N-K+i) */
        binom = (binom * factor) / (seq_t)i;
    }

    /* Factorial ratio (r-c)!/(c+1)! = product_{i=c+2}^{r-c} i */
    seq_t result = binom;
    for (idx_t i = c + 2; i <= r - c; ++i) {
        result *= (seq_t)i;
    }

    return result;
}
