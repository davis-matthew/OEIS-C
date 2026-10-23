/*
 * A158824  Triangle T(n,k) = A000292(n) if k = 1 otherwise
 *          (k-1)*(n-k+1)*(n-k+2)/2, read by rows.
 * https://oeis.org/A158824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,c) where r = minimal positive integer with r(r+1)/2
 *            >= n, c = n - r(r-1)/2. T(r,1) = binom(r+2,3) T(r,c) =
 *            (c-1)*binom(r-c+2,2) for c >= 2 This reads the triangle
 *            row‑by‑row
 * Range:     u8 n <= 28, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------
   Local helpers: integer square root (u64) and binomial
   ------------------------------------------------------------- */

/* integer sqrt: floor(sqrt(x)) for 0 <= x < 2^64 */
static idx_t _isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* 2^32 > sqrt(2^64-1) */
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi  = mid - 1;
        }
    }
    return ans;
}

/* find minimal r such that r*(r+1)/2 >= n */
static idx_t _row_from_n(idx_t n) {
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;                 /* double until sufficient */
    }
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* multiplicative binomial coefficient C(n,k),
   works for any seq_t (built‑in integer or big integer)        */
static seq_t _binom(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;            /* exact division each step */
    }
    return res;
}

/* -------------------------------------------------------------
   Public interface
   ------------------------------------------------------------- */
extern seq_t a(idx_t n) {
    if (n < 1) return 0;              /* offset starts at 1 */

    /* locate row r and column c */
    idx_t r = _row_from_n(n);
    idx_t prev = (r - 1) * r / 2;     /* previous rows total count */
    idx_t c = n - prev;               /* column index, 1‑based */

    if (c == 1) {
        return _binom(r + 2, 3);
    } else {
        seq_t term = _binom(r - c + 2, 2);
        return (seq_t)(c - 1) * term;
    }
}
