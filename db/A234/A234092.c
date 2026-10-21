/*
 * A234092  Limit of v(m,n) as m->oo, where v(m,n) is the number of distinct
 *          terms in the n-th partition of m in Mathematica (lexicographic)
 *          ordering of the partitions of m.
 * https://oeis.org/A234092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + distinct parts of the lexicographically r‑th
 *            partition of x, where x is the smallest integer with Σ_{k=0}^{x}
 *            p(k) ≥ n, and r = n – Σ_{k=0}^{x‑1} p(k)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t idx_t;

/* --------------------------------------------------------------------- */
/* Precomputation of partition numbers and their cumulative sums.       */
/* --------------------------------------------------------------------- */

#define MAX 200                                     /* sufficient for n ≤ ~10⁴ */

static bool init_done = false;
static unsigned long long dp[MAX + 1][MAX + 1];
static unsigned long long p[MAX + 1];
static unsigned long long cum[MAX + 1];

static void init(void)
{
    for (int m = 0; m <= MAX; ++m) {
        dp[0][m] = 1ULL;                            /* one partition of 0 */
    }
    for (int n = 1; n <= MAX; ++n) {
        for (int m = 0; m <= MAX; ++m) {
            unsigned long long a = (m == 0) ? 0ULL : dp[n][m - 1];
            unsigned long long b = (n >= m) ? dp[n - m][m] : 0ULL;
            dp[n][m] = a + b;                       /* standard partition DP */
        }
    }
    for (int n = 0; n <= MAX; ++n) {
        p[n] = dp[n][n];                             /* all partitions of n */
    }
    cum[0] = p[0];
    for (int n = 1; n <= MAX; ++n) {
        cum[n] = cum[n - 1] + p[n];
    }
    init_done = true;
}

/* --------------------------------------------------------------------- */
/* Lexicographically r‑th partition of rem.                               */
/* --------------------------------------------------------------------- */

static void kth_partition(int rem, int max, idx_t r, int *parts, int *len)
{
    int pos = 0;
    while (rem > 0) {
        int start = (rem < max) ? rem : max;
        for (int part = start; part >= 1; --part) {
            unsigned long long cnt = dp[rem - part][part];
            if (r > cnt) {
                r -= cnt;                               /* skip these */
            } else {
                parts[pos++] = part;
                rem -= part;
                max   = part;                           /* maintain non‑increasing order */
                break;
            }
        }
    }
    *len = pos;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.                                               */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (!init_done) init();

    if (n == 0) return 0;                         /* undefined for n = 0 */

    /* find smallest x with cumulative sum ≥ n */
    int x = 0;
    while (x <= MAX && cum[x] < n) ++x;
    if (x > MAX) return 0;                         /* should not happen for n tested */

    unsigned long long prev = (x == 0) ? 0ULL : cum[x - 1];
    idx_t r = (idx_t)(n - prev);                   /* rank within partitions of x */

    int parts[201];
    int len;
    if (x == 0) {
        len = 0;
    } else {
        kth_partition(x, x, r, parts, &len);
    }

    /* count distinct parts of the remainder partition */
    int distinct = 0;
    int last = -1;
    for (int i = 0; i < len; ++i) {
        if (parts[i] != last) {
            ++distinct;
            last = parts[i];
        }
    }

    /* add the leading part, which is distinct from all others */
    return (seq_t)(1 + distinct);
}
