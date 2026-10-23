/*
 * A182263  G.f. satisfies: A(x) = 1 + Sum_{n>=1} x^n/n! * d^n/dx^n x*A(x)^2.
 * https://oeis.org/A182263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1) * sum_{k=0..n-1} a(k)*a(n-1-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static state for memoization */
static seq_t *a_cache = NULL;     /* cached a values */
static idx_t a_cache_len = 0;     /* number of cached entries */
static seq_t pow2_curr = (seq_t)1; /* current power 2^m during construction */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise on first use */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t) * (size_t)1);
        a_cache[0] = (seq_t)1;     /* A(0) */
        a_cache_len = 1;
        pow2_curr = (seq_t)1;      /* 2^0 */
    }

    /* already computed? */
    if (n < a_cache_len)
        return a_cache[n];

    /* extend cache up to n */
    a_cache = (seq_t*)realloc(a_cache, sizeof(seq_t) * (size_t)(n + 1));
    for (idx_t m = a_cache_len; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            sum += a_cache[k] * a_cache[m - 1 - k];
        }

        pow2_curr = pow2_curr * (seq_t)2;          /* 2^m */
        seq_t factor = pow2_curr - (seq_t)1;       /* 2^m - 1 */

        a_cache[m] = factor * sum;
    }
    a_cache_len = n + 1;
    return a_cache[n];
}
