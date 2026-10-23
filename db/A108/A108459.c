/*
 * A108459  Number of labeled partitions of (n,n) into pairs (i,j).
 * https://oeis.org/A108459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^n * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   seq_malloc, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a direct computation of Stirling numbers of the second kind and a
   big‑integer power helper (seq_pow).  The function is safe for any
   allowed seq_t implementation. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    /* The empty partition is counted as one way. */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate space for two rows of Stirling numbers. */
    size_t sz = (size_t)(n + 1);
    seq_t *prev = seq_malloc(sz);
    seq_t *cur  = seq_malloc(sz);

    /* Initialise rows to zero. */
    memset(prev, 0, sz * sizeof(seq_t));
    memset(cur,  0, sz * sizeof(seq_t));
    prev[0] = (seq_t)1;   /* S(0,0) = 1 */

    /* Build S(n,k) row by row. */
    for (idx_t m = (idx_t)1; m <= n; ++m) {
        cur[0] = (seq_t)0;          /* S(m,0) = 0 for m > 0 */
        for (idx_t k = (idx_t)1; k <= m; ++k) {
            /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
            cur[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        /* Prepare for next iteration: swap pointers. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Compute the sum Σ k^n * S(n,k).  k = 0 contributes nothing for n > 0. */
    seq_t result = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t power = seq_pow((seq_t)k, n);
        result += prev[k] * power;
    }

    /* Free the allocated arrays. */
    free((void *)prev);
    free((void *)cur);

    return result;
}
