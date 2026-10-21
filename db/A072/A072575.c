/*
 * A072575  Triangle T(n,k) of number of compositions (ordered partitions) of
 *          n into distinct parts where largest part is exactly k, 1<=k<=n.
 * https://oeis.org/A072575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 1, let r be the smallest integer with r(r+1)/2 ≥ n and
 *            let c = n – r(r–1)/2. Then a(n) = T(r,c), where T(r,k) =
 *            Σ_{U⊆{1,…,k–1}, ΣU = r–k} (|U|+1)! (number of ordered partitions
 *            with distinct parts whose largest part is exactly k)
 * Range:     u8 n <= 177, u16 n <= 712, u32 n <= 3585, u64 n <= 9949,
 *            u128 n <= 9949, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* --------- constants and tables --------- */
#define MAX_N 200      /* enough for index n up to ~10000 (row 141) */

static seq_t dp[MAX_N+1][MAX_N+1];        /* subsets of {1..k-1} */
static seq_t fact_[MAX_N+1];              /* (m)!  for 0 ≤ m ≤ MAX_N */
static seq_t Ttab[MAX_N+1][MAX_N+1];      /* triangle entries T(n,k) */
static bool initialized = false;

/* --------- initialization --------- */
static void init(void)
{
    if (initialized) return;

    /* factorials */
    fact_[0] = 1;
    for (int i = 1; i <= MAX_N; ++i)
        fact_[i] = fact_[i-1] * (seq_t)i;

    /* dp[0][0] = 1 is already zero‑initialised; set it explicitly */
    dp[0][0] = 1;

    /* main DP loops */
    for (int k = 1; k <= MAX_N; ++k)
    {
        /* compute T(n,k) for all n ≥ k */
        for (int n = k; n <= MAX_N; ++n)
        {
            int sumS = n - k;
            seq_t val = 0;
            for (int m = 0; m <= k-1; ++m)
            {
                if (dp[sumS][m] != 0)
                    val += dp[sumS][m] * fact_[m+1];
            }
            Ttab[n][k] = val;
        }

        /* update dp by adding element k to the set {1..k} for next iteration */
        if (k < MAX_N)
        {
            int v = k;                 /* new element */
            for (int sum = MAX_N; sum >= v; --sum)
            {
                for (int m = k-1; m >= 0; --m)
                {
                    if (dp[sum - v][m] != 0)
                        dp[sum][m+1] += dp[sum - v][m];
                }
            }
        }
    }
    initialized = true;
}

/* --------- public function --------- */
seq_t a(idx_t n)
{
    init();                         /* ensure tables ready */

    /* locate (row r, column c) corresponding to the nth term */
    idx_t r   = 1;
    idx_t tri = 1;                  /* r(r+1)/2 for current r */
    while (tri < n)
    {
        ++r;
        tri += r;
    }
    if (r > MAX_N) return 0;        /* out of pre‑computed range */

    idx_t prev = tri - r;           /* triangular number of previous rows */
    idx_t col  = n - prev;          /* column within the row */
    if (col < 1 || col > r) return 0;

    return Ttab[(int)r][(int)col];
}
