/*
 * A157232  Numerator of Euler(n, 3/28).
 * https://oeis.org/A157232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A157232 - Numerator of Euler(n, 3/28)
 * Recurrence derived from the e.g.f. 2*exp(3*x)/(1+exp(28*x)):
 * a(0) = 1
 * a(n) = 3^n - (1/2) * Σ_{m=1}^{n} C(n,m) 28^m a(n-m)   (n ≥ 1)
 * where C(n,m) is the binomial coefficient. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0..cache_len-1] holds a(n) */
static idx_t cache_len = 0;          /* current number of cached entries */

/* Current value of 3^k and the corresponding k */
static seq_t pow3_val = 1; 
static idx_t pow3_k = 0;

/* Ensure cache holds terms up to index n.  All terms are computed lazily
 * and cached, so arbitrary order of calls is supported. */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && cache_len > n)
        return;                       /* already computed */

    /* Allocate / resize cache */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        exit(1);                     /* cannot allocate – abort */
    cache = new_cache;

    /* Initialise the first term if this is the very first call */
    if (cache_len == 0) {
        cache[0] = 1;                 /* a(0) = 1 */
        cache_len = 1;
        pow3_val = 1;                 /* 3^0 */
        pow3_k = 0;
    }

    /* Compute missing entries k = cache_len .. n */
    for (idx_t k = cache_len; k <= n; ++k) {
        /* Advance 3^k */
        while (pow3_k < k) {
            pow3_val = pow3_val * (seq_t)3;
            ++pow3_k;
        }

        /* Compute the convolution sum
           S(k) = Σ_{m=1}^{k} C(k,m) 28^m a(k-m)   */
        seq_t sum = 0;
        seq_t binom = 1;               /* C(k,0) = 1 */
        seq_t pow28 = 1;               /* 28^0 = 1 */

        for (idx_t m = 1; m <= k; ++m) {
            /* Update binomial coefficient C(k,m) */
            binom = binom * (seq_t)(k - m + 1) / (seq_t)m;

            /* Update 28^m */
            pow28 = pow28 * (seq_t)28;

            /* term = C(k,m) * 28^m * a(k-m) */
            seq_t term = binom * pow28 * cache[k - m];
            sum += term;
        }

        /* a(k) = 3^k - sum/2 */
        seq_t half_sum = sum / 2;     /* division is exact */
        cache[k] = pow3_val - half_sum;
    }

    cache_len = n + 1;
}

/* Public function required by the judge */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
