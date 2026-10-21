/*
 * A055434  Number of points in Z^n of norm <= 10.
 * https://oeis.org/A055434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{min(100,n)} C(n,k) * 2^k * S(k) where S(k) is
 *            the number of ordered k‑tuples of positive integers (each
 *            between 1 and 10) whose squared sum is at most 100
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */
/* This implementation uses a dynamic programming table to precompute
   all S(k) for 0 ≤ k ≤ 100.  For each query n we sum the terms
   C(n,k)·2^k·S(k).  The binomial coefficient is computed
   multiplicatively.  All arithmetic is performed with the
   sequence element type `seq_t`, which may be any integer type
   supported by the build. */
#include <stdbool.h>

/* maximum number of non‑zero coordinates that can have a square
   contribution >0 when the radius is 10 */
#define MAXK 100
/* maximum squared radius */
#define R2   100

/* pre‑computed data */
static seq_t S[MAXK + 1];
static seq_t POW2[MAXK + 1];
static bool INITIALIZED = false;

/* multiplicative binomial coefficient C(n,k) */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* pre‑compute S(k) and POW2(k) */
static void init_precomp(void)
{
    if (INITIALIZED) return;
    INITIALIZED = true;

    /* DP table: dp[l][s] = # of sequences of length l with exact sum of squares s */
    static seq_t dp[MAXK + 1][R2 + 1];
    /* dp[0][0] is 1, all others are zero by static initialization */
    dp[0][0] = 1;

    for (int l = 1; l <= MAXK; ++l)
    {
        for (int s = 0; s <= R2; ++s) dp[l][s] = 0;
        for (int s0 = 0; s0 <= R2; ++s0)
        {
            if (dp[l - 1][s0] == 0) continue;
            seq_t base = dp[l - 1][s0];
            for (int t = 1; t <= 10; ++t)
            {
                int w = t * t;
                if (s0 + w <= R2)
                    dp[l][s0 + w] += base;
            }
        }
    }

    for (int l = 0; l <= MAXK; ++l)
    {
        seq_t total = 0;
        for (int s = 0; s <= R2; ++s) total += dp[l][s];
        S[l] = total;
    }

    POW2[0] = 1;
    for (int i = 1; i <= MAXK; ++i) POW2[i] = POW2[i - 1] << 1;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_precomp();

    seq_t result = 0;
    idx_t maxk = (n > MAXK) ? MAXK : (idx_t)n;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin = binom(n, k);
        result += bin * POW2[k] * S[k];
    }

    return result;
}
