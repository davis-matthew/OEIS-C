/*
 * A052521  Number of pairs of sequences of cardinality at least 3.
 * https://oeis.org/A052521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 6, a(n) = (n-5) * n! for n ≥ 6
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *fact_cache = NULL;
static idx_t cache_len = 0;

/* Compute the factorial of n and store it in fact_cache[n] */
static void ensure_factorial(idx_t n) {
    if (cache_len == 0) {
        /* Initialise fact_cache[0] = 1 */
        cache_len = 1;
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        fact_cache[0] = (seq_t)1;
    }
    if (n + 1 <= cache_len) return;           /* already computed up to n */

    /* Expand cache */
    fact_cache = (seq_t *)realloc(fact_cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    if (n < 6) return (seq_t)0;
    ensure_factorial(n);
    seq_t fact_n = fact_cache[n];
    seq_t mult = (seq_t)(n - 5);
    return fact_n * mult;
}
