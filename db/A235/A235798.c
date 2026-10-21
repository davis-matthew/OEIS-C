/*
 * A235798  Triangle read by rows: T(n,k) = number of occurrences of k in all
 *          overpartitions of n.
 * https://oeis.org/A235798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2 * Σ_{m≥0} p_o(r – k – 2km) * where r is the smallest
 *            integer with r(r+1)/2 ≥ n, * k = n – (r‑1)r/2 (position in that
 *            row), * and p_o(m) is the number of overpartitions of m. * The
 *            overpartition numbers are computed by dynamic programming *
 *            using the generating function ∏_{j≥1} (1+x^j)/(1...
 * Range:     u8 n <= 36, u16 n <= 276, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static seq_t *overpartitions = NULL;
static idx_t overpart_max = 0ULL;

static void ensure_overpartitions(idx_t need)
{
    if (need <= overpart_max)
        return;

    free(overpartitions);
    overpartitions = (seq_t *)malloc((need + 1) * sizeof(seq_t));

    seq_t *dp   = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc((need + 1) * sizeof(seq_t));

    for (idx_t i = 0; i <= need; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t j = 1; j <= need; ++j)
    {
        /* copy previous state */
        for (idx_t i = 0; i <= need; ++i)
            next[i] = dp[i];

        /* add contributions of part j */
        for (idx_t shift = j; shift <= need; shift += j)
        {
            for (idx_t i = shift; i <= need; ++i)
                next[i] += (seq_t)2 * dp[i - shift];
        }

        /* swap dp and next */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    for (idx_t i = 0; i <= need; ++i)
        overpartitions[i] = dp[i];

    free(dp);
    free(next);
    overpart_max = need;
}

seq_t a(idx_t n)
{
    /* find row r: (r-1)r/2 < n ≤ r(r+1)/2 */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n)
        ++r;
    while ((r - 1) * r / 2 >= n)
        --r;

    idx_t k = n - (r - 1) * r / 2;   /* 1‑based position in row */

    ensure_overpartitions(r);

    seq_t total = 0;
    for (idx_t m = 0;; ++m)
    {
        long long t_ll = (long long)r - (long long)k - 2LL * (long long)k * (long long)m;
        if (t_ll < 0)
            break;
        idx_t t = (idx_t)t_ll;
        total += (seq_t)2 * overpartitions[t];
    }

    return total;
}
