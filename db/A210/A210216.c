/*
 * A210216  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210215; see the Formula section.
 * https://oeis.org/A210216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial v(r,x) where r
 *            is the smallest integer with r(r+1)/2 ≥ n and k = n - r(r-1)/2 -
 *            1. The polynomials u(n,x), v(n,x) are defined by u(1,x)=v(1,x)=1
 *            u(n,x)=x*u(n-1,x)+v(n-1,x)+1 v(n,x)=x*u(n-1,x)+x*v(n-1,x)+1
 * Range:     u8 n <= 51, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root of a 64‑bit unsigned value (floor). */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (uint64_t)1 << 32;   /* sqrt(2^64-1) < 2^32 */
    uint64_t ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
            ans = mid;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Smallest r such that r(r+1)/2 >= n.  n>=1. */
static inline uint64_t row_of_n(uint64_t n)
{
    uint64_t lo = 1, hi = 0xFFFFFFFFULL;   /* r < 2^32  for 64‑bit n */
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */

    uint64_t nn = (uint64_t)n;
    uint64_t r = row_of_n(nn);

    /* column index in row r (0‑based) */
    uint64_t prev_rows = (unsigned __int128)(r - 1) * r / 2;
    uint64_t k = nn - prev_rows - 1;   /* 0 ≤ k < r */

    size_t maxlen = (size_t)r;

    /* allocate four arrays of size r for intermediate values */
    seq_t *u_prev = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *u_cur  = (seq_t *)malloc(maxlen * sizeof(seq_t));
    seq_t *v_cur  = (seq_t *)malloc(maxlen * sizeof(seq_t));
    if (!u_prev || !v_prev || !u_cur || !v_cur) {
        /* memory failure: return 0 as a fallback */
        free(u_prev); free(v_prev); free(u_cur); free(v_cur);
        return 0;
    }

    /* row 1 initialization */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    size_t prev_len = 1;

    /* Build rows 2 … r */
    for (size_t row = 2; row <= r; ++row) {
        size_t len_new = row;

        /* i = 0 */
        u_cur[0] = v_prev[0] + (seq_t)1;
        v_cur[0] = (seq_t)1;

        /* i = 1 … len_new-1 */
        for (size_t i = 1; i < len_new; ++i) {
            seq_t u_val = u_prev[i - 1];
            if (i < prev_len) u_val += v_prev[i];
            u_cur[i] = u_val;

            v_cur[i] = u_prev[i - 1] + v_prev[i - 1];
        }

        /* swap current and previous arrays */
        seq_t *tmp = u_prev; u_prev = u_cur; u_cur = tmp;
        tmp = v_prev; v_prev = v_cur; v_cur = tmp;
        prev_len = len_new;
    }

    seq_t result = v_prev[(size_t)k];

    free(u_prev);
    free(v_prev);
    free(u_cur);
    free(v_cur);

    return result;
}
