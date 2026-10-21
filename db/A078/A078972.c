/*
 * A078972  Brilliant numbers: semiprimes (products of two primes, A001358)
 *          whose prime factors have the same number of decimal digits.
 * https://oeis.org/A078972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = kth smallest number which is a product of two primes *
 *            (semiprime) whose prime factors have the same number of decimal
 *            digits. * The sequence is sorted in ascending order
 * Range:     u8 n <= 10, u16 n <= 436, u32 n <= 9120, u64 n <= 9120,
 *            u128 n <= 9120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

/* Comparator for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t a = *(const seq_t *)p1;
    const seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* a(n) : returns the nth brilliant number (external linkage) */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range */

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static int current_digit = 0;        /* digit length processed so far */

    /* Grow cache until it contains at least n elements */
    while (cache_size < (size_t)n) {
        ++current_digit;                  /* next digit length */

        /* Compute 10^d and 10^(d-1) in uint64_t (fits for d<=18) */
        uint64_t pow10d = 1;
        for (int i = 0; i < current_digit; ++i) pow10d *= 10ULL;
        uint64_t min_val = pow10d / 10ULL;
        if (min_val < 2ULL) min_val = 2ULL;
        uint64_t limit = pow10d - 1ULL;

        /* Simple sieve up to limit */
        char *is_composite = (char *)calloc(limit + 1, 1);
        if (!is_composite) abort();       /* memory allocation failure */
        for (uint64_t p = 2; p * p <= limit; ++p) {
            if (!is_composite[p]) {
                for (uint64_t m = p * p; m <= limit; m += p)
                    is_composite[m] = 1;
            }
        }

        /* Count primes in the range [min_val, limit] */
        size_t pr_cnt = 0;
        for (uint64_t i = min_val; i <= limit; ++i)
            if (!is_composite[i]) ++pr_cnt;

        /* Store primes in a temporary array */
        uint64_t *primes = (uint64_t *)malloc(pr_cnt * sizeof(uint64_t));
        if (!primes) abort();
        size_t idx = 0;
        for (uint64_t i = min_val; i <= limit; ++i)
            if (!is_composite[i]) primes[idx++] = i;

        /* Generate all products p*q with p <= q */
        size_t prod_cnt = pr_cnt * (pr_cnt + 1ULL) / 2ULL;
        seq_t *products = (seq_t *)malloc(prod_cnt * sizeof(seq_t));
        if (!products) abort();
        size_t pidx = 0;
        for (size_t i = 0; i < pr_cnt; ++i) {
            for (size_t j = i; j < pr_cnt; ++j) {
                products[pidx++] = (seq_t)primes[i] * (seq_t)primes[j];
            }
        }

        /* Sort the new products */
        qsort(products, prod_cnt, sizeof(seq_t), cmp_seq_t);

        /* Ensure cache has enough capacity and append new products */
        if (cache_cap < cache_size + prod_cnt) {
            size_t new_cap = (cache_cap == 0) ? (prod_cnt * 2) : (cache_cap * 2);
            if (new_cap < cache_size + prod_cnt) new_cap = cache_size + prod_cnt;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        memcpy(cache + cache_size, products, prod_cnt * sizeof(seq_t));
        cache_size += prod_cnt;

        /* Clean up temporary arrays */
        free(products);
        free(primes);
        free(is_composite);
    }

    return cache[n - 1];
}
