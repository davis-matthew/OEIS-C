/*
 * A234293  E.g.f. satisfies: A(x) = 1 + A(x)^4 * Integral 1/A(x)^3 dx.
 * https://oeis.org/A234293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence derived from A' = A^2/(4-3A) with a(0)=1, a(1)=1:
 *            a(n+1) = 5*a(n) + Σ_{k=1}^{n-1} C(n,k) * a(k) * a(n-k) +
 *            3*Σ_{k=1}^{n-1} C(n,k) * a(k) * a(n-k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache   = NULL;
static idx_t  cache_size = 0;
static idx_t  last_index = 0;

/* make sure cache can hold indices up to n (inclusive) */
static void ensure_array(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_size;
    if (cache_size == 0) {
        new_size = n + 1;
        if (new_size < 2) new_size = 2;               /* at least two entries */
    } else {
        new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
    }

    seq_t *new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
    if (!new_arr) exit(1);

    if (cache) {
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_size = new_size;

    if (cache_size >= 2) {                         /* first time allocation */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_index = 1;
    } else if (cache_size >= 1) {
        cache[0] = (seq_t)1;
        last_index = 0;
    }
}

/* compute sequence values up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    for (idx_t m = last_index + 1; m <= n; ++m) {
        idx_t prev = m - 1;                    /* recurrence uses a(prev) */
        seq_t a_prev = cache[prev];

        seq_t sum1 = 0;
        seq_t sum2 = 0;
        seq_t binom = 1;                       /* C(prev,0) */

        for (idx_t k = 1; k <= prev - 1; ++k) {
            binom = binom * (seq_t)(prev - k + 1) / (seq_t)k;
            seq_t prod1 = binom * cache[k] * cache[prev - k];
            sum1 += prod1;
            seq_t prod2 = binom * cache[k] * cache[prev - k + 1];
            sum2 += prod2;
        }

        seq_t val = a_prev * (seq_t)5 + sum1 + (sum2 * (seq_t)3);
        cache[m] = val;
    }
    last_index = n;
}

seq_t a(idx_t n)
{
    if (cache_size == 0) ensure_array(n);
    else if (cache_size <= n) ensure_array(n);

    if (n > last_index) compute_up_to(n);
    return cache[n];
}
