/*
 * A084416  Triangle read by rows: T(n,k) = Sum_{i=k..n} i!*Stirling2(n,i), n
 *          >= 1, 1 <= k <= n.
 * https://oeis.org/A084416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the row‑wise flattened triangle T(m,k) where T(m,k) =
 *            Σ_{i=k..m} i! * Stirling2(m,i)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r: smallest r with r*(r+1)/2 >= n */
    uint64_t low = 1, high = n;
    while (low < high) {
        uint64_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    uint64_t r = low;
    uint64_t prev = r * (r - 1) / 2;
    uint64_t k = n - prev;          /* 1‑based column index */

    /* Allocate workspaces: Stirling numbers and factorials */
    seq_t *Sprev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *Scurr = (seq_t *)malloc((r + 2) * sizeof(seq_t));
    seq_t *fact  = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!Sprev || !Scurr || !fact) {
        free(Sprev); free(Scurr); free(fact);
        return (seq_t)0;
    }

    /* Initialise Stirling numbers: S(0,0)=1, others 0 */
    for (uint64_t j = 0; j <= r; ++j)
        Sprev[j] = (seq_t)0;
    Sprev[0] = (seq_t)1;

    /* Compute factorials */
    fact[0] = (seq_t)1;
    for (uint64_t i = 1; i <= r; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* Build Stirling numbers S(i,j) up to i = r */
    for (uint64_t i = 1; i <= r; ++i) {
        Scurr[0] = (seq_t)0;
        for (uint64_t j = 1; j <= i; ++j) {
            /* S(i,j) = j * S(i-1,j) + S(i-1,j-1) */
            Scurr[j] = (seq_t)j * Sprev[j] + Sprev[j-1];
        }
        /* Copy to Sprev for next iteration */
        for (uint64_t j = 0; j <= i; ++j)
            Sprev[j] = Scurr[j];
    }

    /* Compute T(r,k) = Σ_{i=k..r} fact[i] * Sprev[i] */
    seq_t T = (seq_t)0;
    for (uint64_t i = k; i <= r; ++i)
        T += fact[i] * Sprev[i];

    free(Sprev);
    free(Scurr);
    free(fact);

    return T;
}
