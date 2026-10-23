/*
 * A027397  Number of labeled servers of dimension 10.
 * https://oeis.org/A027397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp( Σ_{i=1}^{10} ((1+x)^i - 1)/i )
 *            Recurrence: a(0)=1; for n≥1 a(n) = Σ_{k=1}^{min(10,n)}
 *            binom(10,k) * a(n-k) * (n-1)!/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *memo = NULL;      /* cached values of a(n) */
static idx_t memo_len = 0;      /* number of elements currently stored */
static size_t memo_cap = 0;     /* allocated capacity of memo array */

/* binomial(10,k) for k=1..10 */
static const seq_t C[11] = {0,
    (seq_t)10,  (seq_t)45,  (seq_t)120,
    (seq_t)210, (seq_t)252, (seq_t)210,
    (seq_t)120, (seq_t)45,  (seq_t)10,
    (seq_t)1};

seq_t a(idx_t n)
{
    /* Ensure memoization array is large enough */
    if (n >= memo_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_arr = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!new_arr) abort();
        memo = new_arr;
        memo_cap = new_cap;
    }

    /* Compute missing entries up to n */
    for (idx_t i = memo_len; i <= n; ++i) {
        if (i == 0) {
            memo[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t prod = (seq_t)1;          /* (i-1)!/(i-k)! for current k in loop */
        idx_t max_k = (i < 10 ? i : 10);

        for (idx_t k = 1; k <= max_k; ++k) {
            /* temp = binom(10,k) * a[i-k] * prod */
            seq_t temp = C[k] * memo[i - k];
            temp *= prod;
            sum += temp;

            if (k < max_k) {
                /* Update prod to include factor (i - k) for next k */
                prod *= (i - k);
            }
        }
        memo[i] = sum;
    }
    memo_len = n + 1;
    return memo[n];
}
