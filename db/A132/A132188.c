/*
 * A132188  Number of 3-term geometric progressions with no term exceeding n.
 * https://oeis.org/A132188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k squarefree ≤ n} ⌊√(n/k)⌋² where the sum runs over
 *            all square‑free integers k ≤ n
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root (floor) for idx_t values.  Uses only arithmetic
   without overflow.  Returns 0 for x == 0. */
static idx_t isqrt_idx_t(idx_t x)
{
    if (x == 0) return 0;
    idx_t lo = 1, hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;      /* mid ≥ 1 */
        idx_t div = x / mid;             /* mid * div ≤ x < (mid+1) * (n/(mid+1)) */
        if (mid == div) {
            return mid;
        }
        if (mid < div)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Cache for square‑free flags up to the largest n seen so far. */
static unsigned char *squarefree = NULL;
static idx_t max_n_sq = 0;

/* Recompute the square‑free flag array up to n.  Uses a simple sieve of
   squares.  The array has indices 0..n inclusive. */
static void recompute_squarefree(idx_t n)
{
    unsigned char *new_arr = (unsigned char *)malloc((size_t)(n + 1));
    if (!new_arr) exit(1);
    for (idx_t i = 0; i <= n; ++i)
        new_arr[i] = 1;                     /* assume square‑free */

    idx_t limit = isqrt_idx_t(n);            /* floor(√n) */
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= n; j += sq)
            new_arr[j] = 0;                  /* not square‑free */
    }

    free(squarefree);
    squarefree = new_arr;
    max_n_sq = n;
}

/* Main sequence function.  Computes a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > max_n_sq) recompute_squarefree(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (!squarefree[k]) continue;
        idx_t nk = n / k;
        idx_t t  = isqrt_idx_t(nk);
        sum += (seq_t)t * (seq_t)t;
    }
    return sum;
}
