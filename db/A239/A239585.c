/*
 * A239585  Prime factor <= other prime factor of n-th brilliant number, cf.
 *          A078972.
 * https://oeis.org/A239585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = A020639(A078972(n)), the smaller prime factor of the n-th
 *            brilliant number; computed by sieving the primes of each digit
 *            length, forming all products p*q with p <= q, and sorting
 * Range:     u8 n <= 1429, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 83 terms through n = 9880 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct { uint64_t prod; uint64_t p; } A239585_pair;

static int A239585_cmp(const void *x, const void *y)
{
    const A239585_pair *a1 = (const A239585_pair *)x;
    const A239585_pair *b1 = (const A239585_pair *)y;
    if (a1->prod < b1->prod) return -1;
    if (a1->prod > b1->prod) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static int digit = 0;

    if (n < 1) return (seq_t)0;

    while (cache_size < (size_t)n) {
        ++digit;
        if (digit > 9) abort();
        uint64_t pow10d = 1;
        for (int i = 0; i < digit; ++i) pow10d *= 10ULL;
        uint64_t lo = pow10d / 10ULL;
        if (lo < 2) lo = 2;
        uint64_t hi = pow10d - 1;

        char *comp = (char *)calloc((size_t)hi + 1, 1);
        if (!comp) abort();
        for (uint64_t p = 2; p * p <= hi; ++p)
            if (!comp[p])
                for (uint64_t m = p * p; m <= hi; m += p) comp[m] = 1;

        size_t cnt = 0;
        for (uint64_t i = lo; i <= hi; ++i) if (!comp[i]) ++cnt;
        uint64_t *pr = (uint64_t *)malloc((cnt ? cnt : 1) * sizeof(uint64_t));
        if (!pr) abort();
        size_t k = 0;
        for (uint64_t i = lo; i <= hi; ++i) if (!comp[i]) pr[k++] = i;

        size_t pc = cnt * (cnt + 1) / 2;
        A239585_pair *prods = (A239585_pair *)malloc((pc ? pc : 1) * sizeof(A239585_pair));
        if (!prods) abort();
        size_t t = 0;
        for (size_t i = 0; i < cnt; ++i)
            for (size_t j = i; j < cnt; ++j) {
                prods[t].prod = pr[i] * pr[j];
                prods[t].p = pr[i];
                ++t;
            }
        qsort(prods, pc, sizeof(A239585_pair), A239585_cmp);

        if (cache_cap < cache_size + pc) {
            size_t nc = cache_cap ? cache_cap * 2 : pc * 2;
            if (nc < cache_size + pc) nc = cache_size + pc;
            uint64_t *tmp = (uint64_t *)realloc(cache, nc * sizeof(uint64_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = nc;
        }
        for (size_t i = 0; i < pc; ++i) cache[cache_size + i] = prods[i].p;
        cache_size += pc;

        free(prods);
        free(pr);
        free(comp);
    }
    return (seq_t)cache[n - 1];
}
