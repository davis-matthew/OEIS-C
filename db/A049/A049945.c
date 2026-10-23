/*
 * A049945  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n-1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (sum_{i=1}^{n-1} a(i)) + a(m) ,
 *             m = 2^{p+1} + 2 - n ,   p = floor(log2(n-2))
 * Initial values:
 *   a(1) = 1, a(2) = 1, a(3) = 4
 * This implements the OEIS A049945 recurrence.
 *
 * The function caches computed terms so it works for any call order.
 */

#include <stdlib.h>

static seq_t *A = NULL;      /* a(n) values, 1‑based */
static seq_t *S = NULL;      /* cumulative sums sum_{i=1}^{k} a(i) */
static idx_t  cache_size = 0;/* current allocated size (1‑based) */

seq_t a(idx_t n)
{
    if (n == 0)          /* out of range, but not used by the problem */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1; /* index 0 unused */
        A = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_size * sizeof(seq_t));

        /* Compute terms from the first missing index up to n */
        idx_t start = (cache_size > 0) ? cache_size : 1;
        for (idx_t k = start; k <= n; ++k) {
            if (k == 1) {
                A[k] = (seq_t)1;
                S[k] = A[k];
            } else if (k == 2) {
                A[k] = (seq_t)1;
                S[k] = S[k-1] + A[k];
            } else if (k == 3) {
                A[k] = (seq_t)4;
                S[k] = S[k-1] + A[k];
            } else {
                /* find the smallest power of two greater than k-2 */
                idx_t upper = 1;
                while (upper <= k-2) upper <<= 1U;   /* 2^{p+1} */

                idx_t m = upper + 2 - k;             /* m = 2^{p+1} + 2 - n */
                A[k] = S[k-1] + A[m];               /* a(k) = sum_{1}^{k-1} + a(m) */
                S[k] = S[k-1] + A[k];
            }
        }
        cache_size = new_size;
    }
    return A[n];
}
