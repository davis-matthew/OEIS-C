/*
 * A048993  Triangle of Stirling numbers of 2nd kind, S(n,k), n >= 0, 0 <= k
 *          <= n.
 * https://oeis.org/A048993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, changed
 * Formula:   a(n) is the (n‑th) element of the flattened triangle of Stirling
 *            numbers of the second kind S(m,k). For n ≥ 0, let m = ⌊(√(8n+1)
 *            – 1)/2⌋. Then k = n – m(m+1)/2 and a(n) = S(m,k) with S(0,0)=1,
 *            S(m,0)=0 (m>0) and S(m,k) = k*S(m‑1,k) + S(m‑1,k‑1) for 1 ≤ k ≤
 *            m–1, S(m,m)=1
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 157, u64 n <= 385,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <cmath>
#else
#include <stdlib.h>
#include <math.h>
#endif

seq_t a(idx_t n)
{
    /* find row m and column k corresponding to index n */
    long double s = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t m = (idx_t)((s - 1.0L) / 2.0L);            /* row index */
    idx_t prevSum = m * (m + 1) / 2;                /* elements before row m */
    idx_t k = n - prevSum;                         /* column index */

#ifndef __cplusplus
    /* -------------------------------- C implementation -------------------------------- */
    static seq_t **cache = NULL;   /* array of pointers to rows, cache[i] holds row i */
    static idx_t cache_size = 0;   /* number of rows cached (cache_size = m+1 when m is cached) */

    if (cache_size <= m) {
        size_t newcap = (size_t)(m + 1);
        cache = (seq_t **)realloc(cache, newcap * sizeof(seq_t *));
        for (idx_t i = cache_size; i <= m; ++i) {
            cache[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (!cache[i]) abort();        /* insufficient memory */
            if (i == 0) {
                cache[0][0] = (seq_t)1;
            } else {
                seq_t *prev = cache[i - 1];
                cache[i][0] = (seq_t)0;
                /* compute inner entries using recurrence */
                for (idx_t j = 1; j <= i; ++j) {
                    if (j == i) {
                        /* diagonal element */
                        cache[i][j] = (seq_t)1;
                    } else {
                        cache[i][j] = ((seq_t)j) * prev[j] + prev[j - 1];
                    }
                }
            }
        }
        cache_size = (idx_t)(m + 1);
    }
    return cache[m][k];
#else
    /* -------------------------------- C++ implementation -------------------------------- */
    static std::vector<std::vector<seq_t>> cache;  /* cache of rows */

    while ((idx_t)cache.size() <= m) {
        idx_t i = (idx_t)cache.size();
        std::vector<seq_t> row(i + 1);
        if (i == 0) {
            row[0] = (seq_t)1;
        } else {
            const std::vector<seq_t> &prev = cache[i - 1];
            row[0] = (seq_t)0;
            for (idx_t j = 1; j <= i; ++j) {
                if (j == i) {
                    row[j] = (seq_t)1;
                } else {
                    row[j] = ((seq_t)j) * prev[j] + prev[j - 1];
                }
            }
        }
        cache.push_back(std::move(row));
    }
    return cache[m][k];
#endif
}
