/*
 * A015183  Sum of (Gaussian) q-binomial coefficients for q=-16.
 * https://oeis.org/A015183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0}^{n} [n,k]_{-16} with [n,k]_q = ∏_{j=1}^{k}
 *            (1-q^{n+1-j})/(1-q^j), q = -16 Recurrence: [n,k] = [n-1,k] +
 *            q^{n-k}·[n-1,k-1] (k>0), [n,0]=[n,n]=1 a(0)=1
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool            init        = false;
    static idx_t           cache_max   = 0;    /* largest n already computed   */
    static idx_t           capacity    = 0;    /* current allocated size       */
    static seq_t          *cache_vals  = NULL; /* a(n) values for n ≤ cache_max*/
    static seq_t          *prev_row    = NULL; /* Gaussian row for n-1         */
    static seq_t          *curr_row    = NULL; /* temporary row for current n  */
    static seq_t          *pow_q       = NULL; /* powers of q = -16            */
    static const seq_t     q = (seq_t)-16;     /* constant for powers          */

    /* initialise arrays only once */
    if (!init) {
        capacity   = 1;
        cache_vals = (seq_t *)malloc(capacity * sizeof(seq_t));
        prev_row   = (seq_t *)malloc(capacity * sizeof(seq_t));
        curr_row   = (seq_t *)malloc(capacity * sizeof(seq_t));
        pow_q      = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache_vals || !prev_row || !curr_row || !pow_q)
            return (seq_t)0;                 /* allocation failed   */

        pow_q[0]       = (seq_t)1;           /* q^0 */
        prev_row[0]    = (seq_t)1;           /* [0,0] = 1          */
        cache_vals[0]  = (seq_t)1;           /* a(0) = 1           */
        init           = true;
    }

    /* if already computed, just return */
    if (n <= cache_max)
        return cache_vals[n];

    /* grow power table & coefficient storage to at least n+1 elements */
    while (capacity <= n) {
        idx_t new_cap = capacity * 2;
        /* reallocate powers of q */
        seq_t *new_pow = (seq_t *)realloc(pow_q, new_cap * sizeof(seq_t));
        if (!new_pow) return (seq_t)0;
        for (idx_t i = capacity; i < new_cap; ++i)
            new_pow[i] = new_pow[i - 1] * q; /* q^(i) = q^(i-1)*q */
        pow_q = new_pow;

        /* reallocate row buffers and cache */
        seq_t *new_prev = (seq_t *)realloc(prev_row, new_cap * sizeof(seq_t));
        seq_t *new_curr = (seq_t *)realloc(curr_row, new_cap * sizeof(seq_t));
        seq_t *new_cache = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
        if (!new_prev || !new_curr || !new_cache) return (seq_t)0;
        prev_row   = new_prev;
        curr_row   = new_curr;
        cache_vals = new_cache;

        capacity = new_cap;
    }

    /* compute rows from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* first element of each row is 1 */
        curr_row[0] = (seq_t)1;
        seq_t sum   = curr_row[0];

        for (idx_t k = 1; k <= i; ++k) {
            /* term1 = [i-1,k]  (exists only for k <= i-1) */
            seq_t term1 = (k <= i - 1) ? prev_row[k] : (seq_t)0;
            /* term2 = [i-1,k-1] */
            seq_t term2 = prev_row[k - 1];
            /* multiplier q^(i-k) */
            seq_t mult = pow_q[i - k] * term2;
            curr_row[k] = term1 + mult;
            sum += curr_row[k];
        }

        cache_vals[i] = sum;

        /* swap rows for next iteration */
        seq_t *tmp = prev_row;
        prev_row   = curr_row;
        curr_row   = tmp;
    }

    cache_max = n;
    return cache_vals[n];
}
