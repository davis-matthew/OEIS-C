/*
 * A097910  Number of parts in all compositions of n into distinct parts.
 * https://oeis.org/A097910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊(√(8n+1)-1)/2⌋} k! · k · A008289(n - k(k-1)/2,
 *            k)
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 65, u64 n <= 173,
 *            u128 n <= 485, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008289(n,k) is the number of partitions of n into k distinct parts.
   We compute it by the recurrence q(n,k)=q(n-k,k)+q(n-1,k-1) with
   q(0,0)=1, q(n,0)=0 (n>0), q(0,k)=0 (k>0). */

#include <stddef.h>
#include <stdlib.h>

static idx_t maxN = 0;          /* largest n computed so far          */
static size_t kmax = 0;         /* maximum k used for current data    */
static seq_t *q = NULL;         /* q[m*(kmax+1)+k]  (partitions)      */
static seq_t *a_cache = NULL;   /* a_cache[n] = a(n)                   */
static seq_t *fac = NULL;       /* fac[k] = k!                         */

/* Recompute tables up to newMaxN */
static void recompute(idx_t newMaxN)
{
    /* compute new maximum k such that k(k+1)/2 ≤ newMaxN */
    idx_t kmax_tmp = 0;
    while ((kmax_tmp + 1) * (kmax_tmp + 2) / 2 <= newMaxN)
        ++kmax_tmp;
    size_t new_kmax = (size_t)kmax_tmp + 1;          /* stride = columns = kmax+1 */

    size_t rows = (size_t)newMaxN + 1;
    size_t cols = new_kmax;
    size_t total = rows * cols;

    /* free old tables if any */
    if (q)          free(q);
    if (a_cache)    free(a_cache);
    if (fac)        free(fac);

    /* allocate new tables */
    q = (seq_t*)calloc(total, sizeof(seq_t));
    a_cache = (seq_t*)calloc(rows, sizeof(seq_t));
    fac = (seq_t*)calloc(new_kmax, sizeof(seq_t));

    /* factorials up to new_kmax-1 */
    fac[0] = 1;
    for (size_t i = 1; i < new_kmax; ++i)
        fac[i] = fac[i-1] * (seq_t)i;

    /* compute q[m][k] */
    for (size_t m = 0; m < rows; ++m) {
        for (size_t k = 0; k < cols; ++k) {
            size_t idx = m * cols + k;
            if (m == 0 && k == 0) {                /* base case */
                q[idx] = 1;
                continue;
            }
            if (k == 0 || m == 0) {                /* impossible partitions */
                q[idx] = 0;
                continue;
            }
            seq_t val = 0;
            if (m >= k)                           /* q[m-k][k] */
                val += q[(m - k) * cols + k];
            /* q[m-1][k-1] */
            val += q[(m - 1) * cols + (k - 1)];
            q[idx] = val;
        }
    }

    /* compute a_cache[n] for n = 1 .. newMaxN */
    for (size_t n = 1; n < rows; ++n) {
        seq_t sum = 0;
        for (size_t kk = 1; kk < cols; ++kk) {
            idx_t sub = (idx_t)kk * (idx_t)(kk - 1) / 2;    /* k(k-1)/2 */
            if ((idx_t)n < sub) break;                  /* higher k impossible for this n */
            size_t m = (size_t)n - (size_t)sub;
            seq_t count = q[m * cols + kk];
            /* multiply in order that avoids overflow when count==0 */
            sum += count * fac[kk] * (seq_t)kk;
        }
        a_cache[(size_t)n] = sum;
    }

    /* update global state */
    maxN = newMaxN;
    kmax = new_kmax - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* sequence is defined for n≥1 */
    if (n > maxN)
        recompute(n);
    return a_cache[(size_t)n];
}
