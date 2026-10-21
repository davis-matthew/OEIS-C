/*
 * A087549  a(1) = 1, a(2) = 1, a(n) = sum of tau(n) previous terms, where
 *          tau(n) is the number of divisors of n.
 * https://oeis.org/A087549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 1, a(n) = Σ_{k=1}^{τ(n)} a(n−k), where τ(n) is
 *            the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache for computed terms */
static seq_t *seq_cache  = NULL; /* cache[0] unused; cache[1].. cache[max_computed] valid */
static size_t  cache_size = 0;    /* allocated size (count of elements) */
static idx_t   max_computed = 0;  /* maximum n with a(n) stored in cache */

/* number of divisors of n (O(sqrt(n)) algorithm) */
static inline idx_t divisor_count(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            ++cnt;
            if (d != n / d) ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n < 1 is unspecified */
    if (n == 0) return 0;

    /* Ensure cache can hold indices up to n (plus index 2 for safety) */
    size_t required = (size_t)(n + 2);
    if (cache_size < required) {
        seq_t *tmp = (seq_t *)realloc(seq_cache, required * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed – return 0 as a fallback */
        seq_cache   = tmp;
        cache_size = required;
    }

    /* Initialise the first two terms if they are not already present */
    if (max_computed < 1) {
        seq_cache[1] = (seq_t)1;
        max_computed = 1;
    }
    if (max_computed < 2) {
        seq_cache[2] = (seq_t)1;
        max_computed = 2;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t tau = divisor_count(i);
        /* τ(i) cannot exceed i‑1 when summing previous terms */
        if (tau > i - 1) tau = i - 1;
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= tau; ++k) {
            sum += seq_cache[i - k];
        }
        seq_cache[i] = sum;
    }

    max_computed = n;
    return seq_cache[n];
}
