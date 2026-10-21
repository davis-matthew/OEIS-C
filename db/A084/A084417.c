/*
 * A084417  Triangle read by rows: T(n,k) = Sum_{i=1..k} (n+1-i)! *
 *          Stirling2(n,n+1-i), n>=1, 1<=k<=n.
 * https://oeis.org/A084417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth element of the triangle T(n,k) = Sum_{i=1..k}
 *            (n+1-i)! * Stirling2(n, n+1-i) with row r determined by
 *            r*(r+1)/2 ≥ n and column k = n - r*(r-1)/2
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_R 200                    /* enough for n≈10000 (row≈141) */

/*  pre‑computed factorials and Stirling numbers of the second kind  */
static seq_t fact[MAX_R + 1];
static seq_t stir[MAX_R + 1][MAX_R + 1];
static bool init_done = false;
static idx_t precomputed_max = 0;

/*  initialise factorials and Stirling numbers up to need  */
static void init_precompute(idx_t need)
{
    if (need <= precomputed_max)
        return;

    if (!init_done) {
        /* base cases */
        fact[0] = 1;
        stir[0][0] = 1;
        for (idx_t i = 1; i <= MAX_R; ++i)
            stir[0][i] = 0;
        init_done = true;
    }

    /* compute factorials */
    for (idx_t i = precomputed_max + 1; i <= need; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* compute Stirling numbers */
    for (idx_t n = precomputed_max + 1; n <= need; ++n) {
        stir[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k)
            stir[n][k] = stir[n - 1][k] * (seq_t)k + stir[n - 1][k - 1];
    }

    precomputed_max = need;
}

/*  compute a single triangle entry T(r,k)  */
static seq_t T_value(idx_t r, idx_t k)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t t = r + 1 - i;           /* factorial and Stirling index  */
        sum += fact[t] * stir[r][t];
    }
    return sum;
}

/*  required public function  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n<1; return 0 safely */

    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;            /* 1‑based column within the row */

    init_precompute(r);

    return T_value(r, k);
}
