/*
 * A170719  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 49: a(n) = 2 + 36 · Σ_{k=0}^{n-1} a(k) For
 *            n = 50: a(50) = 1 + 36 · Σ_{k=1}^{49} a(k) – 666 · a(0) For n ≥
 *            51: a(n) = 36 · Σ_{k=1}^{49} a(n-k) – 666 · a(n-50)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* uint64_t */

/* External integer types supplied by the build */
typedef uint64_t idx_t;

/* Static cache for computed values */
static seq_t *a_arr  = NULL;   /* a(n) values                     */
static seq_t *cum_arr = NULL;  /* cumulative sums cum[n] = Σ_{i=0}^n a(i) */
static size_t cache_cap = 0;   /* allocated capacity (in elements) */
static size_t cache_len = 0;   /* number of values currently stored (len = last_index+1) */

/* Ensure the cache can hold indices up to `needed`. */
static void ensure_capacity(size_t needed)
{
    if (needed + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed + 1)
        new_cap <<= 1;

    seq_t *new_a  = (seq_t *)realloc(a_arr,  new_cap * sizeof(seq_t));
    seq_t *new_cu = (seq_t *)realloc(cum_arr, new_cap * sizeof(seq_t));
    if (!new_a || !new_cu) {
        /* On allocation failure, abort to avoid undefined behaviour. */
        abort();
    }
    a_arr   = new_a;
    cum_arr = new_cu;
    cache_cap = new_cap;
}

/* Main function requested for the OEIS sequence */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* n is guaranteed to be small enough for size_t */

    ensure_capacity(N);

    /* Initialise cache if empty */
    if (cache_len == 0) {
        a_arr[0]   = (seq_t)1;
        cum_arr[0] = a_arr[0];
        cache_len  = 1;
    }

    /* Compute value(s) up to N if not already done */
    for (size_t i = cache_len; i <= N; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i <= 49) {                 /* 1 ≤ i ≤ 49 */
            val = (seq_t)2 + (seq_t)36 * cum_arr[i - 1];
        } else if (i == 50) {                 /* i = 50 */
            seq_t sum_1_49 = cum_arr[49] - a_arr[0];
            val = (seq_t)1 + (seq_t)36 * sum_1_49 - (seq_t)666 * a_arr[0];
        } else {                               /* i ≥ 51 */
            seq_t sum_last49 = cum_arr[i - 1] - cum_arr[i - 50];
            val = (seq_t)36 * sum_last49 - (seq_t)666 * a_arr[i - 50];
        }

        a_arr[i] = val;
        if (i == 0)
            cum_arr[i] = val;
        else
            cum_arr[i] = cum_arr[i - 1] + val;
    }

    cache_len = N + 1;   /* we now have values from 0 to N inclusive */
    return a_arr[N];
}
