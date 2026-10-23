/*
 * A179243  Numbers that have three terms in their Zeckendorf representation.
 * https://oeis.org/A179243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2779, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generates all numbers that have exactly three terms
   in their Zeckendorf representation by enumerating all sums of
   three distinct, non‑consecutive Fibonacci numbers.
   The sequence a(n) is returned for n ≥ 1. */
#include <stddef.h>
#include <stdlib.h>

/* comparison function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* static cache of the sorted, unique sums */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;

/* initialize the cache on first call */
static void init_cache(void)
{
    if (cache != NULL) return;   /* already initialized */

    /* choose enough Fibonacci indices to cover >> 10 000 terms */
    /* number of valid triples for m = 60 is C(58,3) = 30856 > 10 000 */
    const size_t m = 60;

    /* Fibonacci numbers up to index m-1 */
    seq_t fib[m];
    fib[0] = (seq_t)1;   /* F(1) = 1 */
    fib[1] = (seq_t)2;   /* F(2) = 2 */
    for (size_t i = 2; i < m; ++i) fib[i] = fib[i - 1] + fib[i - 2];

    /* maximum number of triples with no consecutive indices:
         C(m-2, 3) */
    size_t max_triples = 0;
    if (m > 2) {
        max_triples = ((m - 2) * (m - 3) * (m - 4)) / 6 + 10;
    }

    cache = (seq_t *)malloc(sizeof(seq_t) * max_triples);
    cache_cap = max_triples;
    size_t pos = 0;

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i + 2; j < m; ++j) {
            for (size_t k = j + 2; k < m; ++k) {
                cache[pos++] = fib[i] + fib[j] + fib[k];
            }
        }
    }

    /* sort the sums */
    qsort(cache, pos, sizeof(seq_t), cmp_seq);

    /* remove duplicates (shouldn't occur but be safe) */
    size_t uniq = 0;
    for (size_t i = 0; i < pos; ++i) {
        if (uniq == 0 || cache[i] != cache[uniq - 1]) {
            cache[uniq++] = cache[i];
        }
    }
    cache_size = uniq;
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range for this sequence */
    init_cache();                 /* guarantee cache is ready */

    if ((idx_t)(cache_size) < n) {
        /* n is larger than we pre‑computed – not expected for k ≤ 10 000 */
        return (seq_t)0;
    }
    return cache[n - 1];
}
