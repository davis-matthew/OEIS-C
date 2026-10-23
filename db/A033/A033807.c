/*
 * A033807  Product t2(q^d); d | 48, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|48} (∑_{k≥0} q^{d·k·(k+1)/2})
 *            (Product over the 10 positive divisors of 48)
 * Range:     u8 n <= 41, u16 n <= 206, u32 n <= 3512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
# define ALLOC_SEQ(n)   (new seq_t[(n)])
# define FREE_SEQ(p)    delete[] (p)
#else
# define ALLOC_SEQ(n)   ((seq_t*)calloc((size_t)(n), sizeof(seq_t)))
# define FREE_SEQ(p)    free(p)
#endif

/* the 10 divisors of 48 */
static const idx_t divisors[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 48};
static const size_t num_divisors = sizeof(divisors)/sizeof(divisors[0]);

/* memoised table: dp[n] = a(n) for n < dp_size */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

/* Compute all a(m) for 0 ≤ m ≤ N and store them in dp */
static void recompute(idx_t N)
{
    size_t sz = (size_t)N + 1;                     /* number of entries */

    /* Allocate two temporary tables */
    seq_t *cur  = ALLOC_SEQ(sz);
    seq_t *next = ALLOC_SEQ(sz);

    /* Initialise cur to the constant 1 polynomial */
    for (size_t i = 0; i < sz; ++i) cur[i] = (seq_t)0;
    cur[0] = (seq_t)1;

    /* Iterate over the divisors producing the product */
    for (size_t d_ix = 0; d_ix < num_divisors; ++d_ix) {
        idx_t d = divisors[d_ix];

        /* clear next before using it */
        for (size_t i = 0; i < sz; ++i) next[i] = (seq_t)0;

        /* convolution: next[ i + w ] += cur[i] where w = d·k·(k+1)/2 */
        for (idx_t i = 0; i <= N; ++i) {
            seq_t ci = cur[(size_t)i];
            if (ci == (seq_t)0) continue;
            for (idx_t k = 0; ; ++k) {
                idx_t w = d * k * (k + 1) / 2;
                if (w > N - i) break;
                next[(size_t)(i + w)] += ci;
            }
        }

        /* Prepare for next divisor: cur = next, next = old cur */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* cur now holds the final coefficient array */
    seq_t *old_dp = dp;
    dp = cur;          /* dp now owns cur */
    dp_size = sz;      /* dp is valid for indices < dp_size */

    /* Free the old table and the buffer that was swapped out as next */
    if (old_dp) FREE_SEQ(old_dp);
    FREE_SEQ(next);    /* next is the buffer that was swapped out */
}

seq_t a(idx_t n)
{
    if (n >= dp_size) {
        recompute(n);
    }
    return dp[(size_t)n];
}
