/*
 * A163982  Real part of the coefficient [x^n] of the expansion of
 *          (1+i)/(1-i*exp(x)) - 1 multiplied by 2*n!, where i is the
 *          imaginary unit.
 * https://oeis.org/A163982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(floor(n/2)+1) * A000111(n), n ≥ 1 a(0) = -2 where
 *            A000111(n) are the Euler zigzag (up/down) numbers. A000111(0)=1
 *            and for m≥0 A000111(m+1) = Σ_{k=0}^{m} C(m,k) * (k even?
 *            A000111(k) : 0) * A000111(m-k) (C(m,k) is the binomial
 *            coefficient). This recurrence follows from the differentia...
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 23, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Global cache for the Euler zigzag numbers */
static seq_t *euler_cache   = NULL;
static idx_t  cache_size    = 0;   /* number of allocated elements       */
static idx_t  cache_computed = 0;  /* number of values that are already filled (≥ 1) */

static void ensure_euler(idx_t n)
{
    /* n will never be smaller than 1 here (handled separately in a()) */
    if (cache_computed > n) return;

    /* enlarge the cache */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(euler_cache, new_size * sizeof(seq_t));
    euler_cache = tmp;
    /* zero the new region */
    if (cache_computed < new_size) {
        memset(euler_cache + cache_computed, 0,
               (new_size - cache_computed) * sizeof(seq_t));
    }

    /* make sure euler_cache[0] is initialized */
    if (cache_computed == 0) {
        euler_cache[0] = (seq_t)1;
        cache_computed = 1;
    }

    /* fill missing values up to n */
    for (idx_t i = cache_computed; i <= n; ++i) {
        idx_t m = i - 1;                /* m = n-1 in the recurrence   */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;            /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            if ((k & 1) == 0) {  /* only even k contribute   */
                seq_t term = bin * euler_cache[k] * euler_cache[m - k];
                sum += term;
            }
            if (k < m) {
                bin = bin * (seq_t)(m - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        euler_cache[i] = sum;
    }
    cache_computed = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)-2;

    ensure_euler(n);
    seq_t val = euler_cache[n];

    /* sign = (-1)^(floor(n/2)+1) */
    idx_t exp = (n >> 1) + 1;
    if (exp & 1U) val = -val;

    return val;
}
