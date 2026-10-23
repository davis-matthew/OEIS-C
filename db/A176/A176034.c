/*
 * A176034  Difference between product of two distinct primes and next perfect
 *          square.
 * https://oeis.org/A176034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = difference between the n-th product of two distinct
 *            primes and the next perfect square greater than that product,
 *            starting at n = 1
 * Range:     u8 none, u16 none, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

typedef struct { seq_t prod; seq_t diff; } ProdDiff;

static int cmp_prod(const void *a, const void *b)
{
    const ProdDiff *x = (const ProdDiff *)a;
    const ProdDiff *y = (const ProdDiff *)b;
    if (x->prod < y->prod) return -1;
    if (x->prod > y->prod) return 1;
    return 0;
}

/* Increments the size of a pointer-to-array, returning the new pointer.  */
static void *grow_array(void *ptr, size_t *cap, size_t need, size_t elem_size)
{
    size_t newcap = *cap ? *cap : 8;
    while (newcap < need) newcap <<= 1;
    void *p = realloc(ptr, newcap * elem_size);
    *cap = newcap;
    return p;
}

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    if (n == 0) { /* offset 1; any call with n<1 is undefined */
        return (seq_t)0;
    }

    /* Expand cache if necessary */
    if ((size_t)n > cache_len) {
        /* Determine how many prime pairs are needed */
        size_t need_pairs = (size_t)n;
        size_t m = 1;
        while (m * (m - 1) / 2 < need_pairs) ++m;   /* minimal number of primes */

        /* Rough upper bound for the m‑th prime: p_m < m*(ln m + ln ln m),
           but for small m we simply use a generous linear bound. */
        size_t prime_limit = m * 20 + 100;

        /* Build primes up to the estimate */
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto((idx_t)prime_limit, &primes, &prime_cnt);

        /* If we didn't get enough primes, increase limit and rebuild */
        while (prime_cnt < m) {
            prime_limit += 200;   /* grow the limit */
            free(primes);
            primes = NULL;
            prime_cnt = 0;
            build_primes_upto((idx_t)prime_limit, &primes, &prime_cnt);
        }

        /* Allocate array for all prime products */
        size_t prod_cnt = prime_cnt * (prime_cnt - 1) / 2;
        ProdDiff *prods = (ProdDiff *)malloc(prod_cnt * sizeof(ProdDiff));

        /* Generate all pairwise products and their differences */
        size_t idx = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            for (size_t j = i + 1; j < prime_cnt; ++j) {
                seq_t p1 = (seq_t)primes[i];
                seq_t p2 = (seq_t)primes[j];
                seq_t prod = p1 * p2;

                seq_t s = floor_sqrt(prod);
                seq_t next_s = s + (seq_t)1;
                seq_t diff = next_s * next_s - prod;

                prods[idx].prod = prod;
                prods[idx].diff = diff;
                ++idx;
            }
        }

        /* Sort the products in ascending order */
        qsort(prods, prod_cnt, sizeof(ProdDiff), cmp_prod);

        /* Ensure the cache is large enough */
        cache = (seq_t *)grow_array(cache, &cache_cap, prod_cnt, sizeof(seq_t));
        for (size_t i = 0; i < prod_cnt; ++i) {
            cache[i] = prods[i].diff;
        }
        cache_len = prod_cnt;

        /* Clean up temporary arrays */
        free(primes);
        free(prods);
    }

    /* Return the required term */
    return cache[(size_t)n - 1];
}
