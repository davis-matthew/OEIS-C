/*
 * A121466  Triangle read by rows: T(n,k) = is the number of directed
 *          column-convex polyominoes of area n having along the lower contour
 *          exactly k reentrant corners, i.e., a vertical step that is
 *          followed by a horizontal step (n >= 1, k >= 0).
 * https://oeis.org/A121466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A121466 – Triangle read by rows
 *
 *  T(n,k) = number of directed column‑convex polyominoes of area n
 *  having exactly k re‑entrant corners (0 ≤ k < ceil(n/2)).
 *
 *  This function returns the flattened triangle:
 *  a(1)=T(1,0), a(2)=T(2,0), a(3)=T(3,0), a(4)=T(3,1), a(5)=T(4,0), …
 *
 *  Formula for T(n,k) (k>=1):
 *      T(n,k) = ∑_{j=0}^{n-2k-1} 2^j * C(n-k-2-j, k-1) * C(k+j, k)
 *  for k=0:  T(n,0) = 2^{n-1}
 *
 *  The implementation uses simple integer arithmetic so it works for
 *  any seq_t type (8,16,32,64 or arbitrary‑precision).
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* utility: integer square root for idx_t (uint64_t) using binary search
 * returns floor(sqrt(x))                                                */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;           /* 2^32 > sqrt(UINT64_MAX) */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;   /* hi is the largest value with hi^2 <= x */
}

/* ------------------------------------------------------------------ */
/* binary coefficient C(n,k) – multiplicative, no factorials          */
static seq_t binomial_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* power of two: 2^exp – shift left, works for any seq_t                 */
static seq_t pow2_exp(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r <<= 1;
    return r;
}

/* ------------------------------------------------------------------ */
/* main function – returns a(n) for the flattened triangle            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* out of range */

    /* 1. find the row r and column k (0‑based) for index n            */
    idx_t k0 = isqrt_idx(n);                    /* floor(sqrt(n))        */

    /* adjust k0 when n lies in the even‑row interval                */
    seq_t lim = (seq_t)k0 * ((seq_t)k0 + 1);
    if ((seq_t)n > lim) ++k0;

    idx_t k = k0;                                 /* row number r = 2*k or 2*k-1 */

    seq_t sqk = (seq_t)k * (seq_t)k;
    idx_t r = ((seq_t)n <= sqk) ? 2*k - 1 : 2*k;

    /* cumulative count up to previous row */
    seq_t s_prev;
    if (r & 1) {                                 /* r odd  => r = 2*m+1   */
        s_prev = (seq_t)(k - 1) * (seq_t)k;      /* ((k-1)*k)          */
    } else {                                     /* r even => r = 2*m     */
        s_prev = (seq_t)k * (seq_t)k;
    }

    seq_t offset = (seq_t)n - s_prev;           /* 1‑based offset in row */
    idx_t col = (idx_t)(offset - 1);            /* 0‑based column number */

    /* 2. compute T(r,col)                                            */
    if (col == 0) return pow2_exp(r - 1);        /* T(r,0) = 2^{r-1}      */

    idx_t max_j = r - 2*col - 1;                 /* upper bound for j      */
    seq_t result = 0;
    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term = pow2_exp(j);
        term *= binomial_idx(r - col - 2 - j, col - 1);
        term *= binomial_idx(col + j, col);
        result += term;
    }
    return result;
}
