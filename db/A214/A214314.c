/*
 * A214314  Number triangle with entry T(n,m) giving the position of the first
 *          partition of n with m parts in the Abramowitz-Stegun (A-St)
 *          partition ordering.
 * https://oeis.org/A214314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = 1 + ∑_{k=1}^{m-1} p(n,k) where p(n,k) counts partitions
 *            of n into exactly k parts. The sequence a(n) is the flattening
 *            of T(n,m) in row‑major order (offset 1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
static idx_t
find_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Return the nth term of A214314. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset is 1 */

    /* Determine (row r, column m) in the triangle. */
    idx_t r = find_row(n);
    idx_t base = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
    idx_t m = n - base;              /* 1 ≤ m ≤ r */

    /* Allocate triangular array p[0..r][0..*] for exact partition counts. */
    seq_t **p = (seq_t**)calloc((size_t)(r + 1), sizeof(seq_t*));
    if (!p) abort();

    for (idx_t i = 0; i <= r; ++i) {
        p[i] = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!p[i]) { free(p); abort(); }
    }

    /* Base: p(0,0)=1; all other p[n][0] remain 0 (default). */
    p[0][0] = (seq_t)1;

    /* Compute p[n][k] for 1 ≤ k ≤ n using the recurrence p(n,k)=p(n-1,k-1)+p(n-k,k). */
    for (idx_t nn = 1; nn <= r; ++nn) {
        for (idx_t kk = 1; kk <= nn; ++kk) {
            seq_t val = 0;
            val += p[nn - 1][kk - 1];
            if (nn >= kk) {
                if (nn - kk >= kk)
                    val += p[nn - kk][kk];
            }
            p[nn][kk] = val;
        }
    }

    /* Compute T(r,m) = 1 + Σ_{k=1}^{m-1} p(r,k). */
    seq_t result = (seq_t)1;
    for (idx_t k = 1; k < m; ++k)
        result += p[r][k];

    /* Free allocated memory. */
    for (idx_t i = 0; i <= r; ++i)
        free(p[i]);
    free(p);

    return result;
}
