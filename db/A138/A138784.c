/*
 * A138784  a(n) = n*(n-1)*(n-2)(27*n^3 - 174*n^2 + 511*n - 552)*n!/48.
 * https://oeis.org/A138784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(27*n^3 - 174*n^2 + 511*n - 552)*n!/48
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;
static idx_t fact_max = 0;
static size_t fact_cap = 0;

static void ensure_factorial(idx_t n)
{
    if (n <= fact_max)
        return;

    size_t need = (size_t)n + 1;
    if (fact_cap < need) {
        size_t new_cap = fact_cap ? fact_cap : 1;
        while (new_cap < need)
            new_cap <<= 1;
        fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }

    if (fact_max == 0)
        fact_cache[0] = (seq_t)1;    /* 0! = 1 */

    for (idx_t i = fact_max + 1; i <= n; ++i)
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

    fact_max = n;
}

static seq_t factorial_cached(idx_t n)
{
    ensure_factorial(n);
    return fact_cache[n];
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t part   = n_seq * (n_seq - 1) * (n_seq - 2);
    seq_t poly   = 27 * n_seq * n_seq * n_seq
                 - 174 * n_seq * n_seq
                 + 511 * n_seq
                 - 552;

    seq_t prod   = part * poly;
    prod /= (seq_t)48;
    prod *= factorial_cached(n);

    return prod;
}
