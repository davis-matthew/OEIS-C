/*
 * A129949  a(n) = n! - binomial(n,3).
 * https://oeis.org/A129949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! - binomial(n,3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;
static idx_t fact_len = 0;

/* Ensure the factorial cache contains values up to n (inclusive) */
static void ensure_fact(idx_t n)
{
    if (n + 1 <= fact_len)
        return;

    size_t new_len = (size_t)(n + 1) * 2;
    seq_t *tmp = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1); /* allocation failure */
    fact_cache = tmp;

    if (fact_len == 0) {
        fact_cache[0] = (seq_t)1;
        fact_len = 1;
    }

    for (idx_t i = fact_len; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_fact(n);
    seq_t fact_n = fact_cache[n];

    seq_t binom = 0;
    if (n >= 3) {
        seq_t t = (seq_t)n * (n - 1) * (n - 2);
        binom = t / 6;
    }

    return fact_n - binom;
}
