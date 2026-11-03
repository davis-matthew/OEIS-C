/*
 * A133332  Triangle read by rows giving coefficients in expansion of
 *          (1+x+x^2+...+x^(n-2))^n in powers of x.
 * https://oeis.org/A133332
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 106, u32 n <= 419, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 964 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A133332
 *  Triangle read by rows giving coefficients in expansion of
 *  (1+x+x^2+...+x^(n-2))^n in powers of x.
 *  a(n) is the n-th term of the flattened list (offset 2).
 *  For a given index n≥2 this function returns
 *  the number of r-tuples (a1,…,ar) with 0≤ai≤r-2
 *  and a1+…+ar = p, where r is the row containing n
 *  and p is the position inside that row (0‑based).
 *  Implementation uses DP to count restricted compositions,
 *  which works for all build sizes that provide the
 *  seq_t type and the binom helper.
 */

#include <stdbool.h>

static seq_t coeff_row_p(idx_t r, idx_t p)
{
    /* r >= 2, 0 <= p < r*(r-2)+1 */
    const idx_t U = r - 2;           /* maximum part value */
    const idx_t MAX_LEN = 1024;      /* enough for r ≤ 32 (p ≤ 960) */
    seq_t dp[MAX_LEN];
    seq_t ndp[MAX_LEN];

    /* initialise */
    for (idx_t i = 0; i <= p; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = 0; part < r; ++part) {
        for (idx_t i = 0; i <= p; ++i) ndp[i] = 0;
        for (idx_t s = 0; s <= p; ++s) {
            if (dp[s] == 0) continue;
            for (idx_t v = 0; v <= U; ++v) {
                idx_t ns = s + v;
                if (ns <= p) ndp[ns] += dp[s];
            }
        }
        for (idx_t i = 0; i <= p; ++i) dp[i] = ndp[i];
    }
    return dp[p];
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* flatten index (0-based) */
    idx_t idx0 = n - 2;

    /* find row r such that cumulative length up to r-1 ≤ idx0 < up to r */
    idx_t r = 2;
    idx_t cum = 0;
    while (1) {
        idx_t len = r * (r - 2) + 1;
        if (cum + len > idx0) break;
        cum += len;
        ++r;
    }

    idx_t p = idx0 - cum;          /* position inside the current row */

    return coeff_row_p(r, p);
}
