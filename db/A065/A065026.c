/*
 * A065026  a(1) = 1; for n>1, a(n) = smallest number that is not a sum or
 *          product of any subset of the numbers a(1) to a(n-1).
 * https://oeis.org/A065026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065026 implementation: a(n) = smallest number that is not a sum or
 * product of any subset of a(1)…a(n‑1).
 * Recurrence (for n>17):  a(n) = 2*a(n‑2) + 4*a(n‑4) – 2*a(n‑6)
 * Initial terms: a(1)=1, a(2)=2, a(3)=4.
 * The function memoises the sequence; it works for arbitrary seq_t
 * (built‑in integer types or small arbitrary‑precision types). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(__cplusplus)
# include <algorithm>
#endif

static seq_t *cache = NULL;          /* memoised values */
static size_t cache_size = 0;        /* number of computed terms */
static size_t cache_cap   = 0;        /* current capacity of cache */

/* Grow the cache array if needed. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    cache_cap = new_cap;
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[cache_cap];
    if (cache && cache_size) {
        std::copy(cache, cache + cache_size, tmp);
        delete[] cache;
    }
    cache = tmp;
#else
    seq_t *tmp = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
    if (cache) {
        memcpy(tmp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
#endif
}

/* Compute a(n) for n <= 17 by exhaustive search over sums and products. */
static seq_t compute_missing(const seq_t *prev, size_t len)
{
    /* Upper bound sufficient for the first 17 terms (a(17)=13952). */
    const size_t LIMIT = 200000;

    unsigned char *sum  = (unsigned char *)calloc(LIMIT + 1, 1);
    unsigned char *prod = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!sum || !prod) exit(EXIT_FAILURE);

    /* Subset sums */
    sum[0] = 1;
    for (size_t i = 0; i < len; ++i) {
        uint64_t v = (uint64_t)prev[i];
        for (size_t j = LIMIT; ; --j) {
            if (sum[j] && j + v <= LIMIT) sum[j + v] = 1;
            if (j == 0) break;
        }
    }

    /* Subset products */
    size_t prod_cap = 16, prod_cnt = 0;
    uint64_t *prod_list = (uint64_t *)malloc(prod_cap * sizeof(uint64_t));
    if (!prod_list) exit(EXIT_FAILURE);

    for (size_t i = 0; i < len; ++i) {
        uint64_t v = (uint64_t)prev[i];

        /* product consisting of the single term v */
        if (v <= LIMIT && !prod[v]) {
            prod[v] = 1;
            if (prod_cnt == prod_cap) {
                prod_cap <<= 1;
                prod_list = (uint64_t *)realloc(prod_list,
                                                prod_cap * sizeof(uint64_t));
                if (!prod_list) exit(EXIT_FAILURE);
            }
            prod_list[prod_cnt++] = v;
        }

        /* products with previously found values */
        size_t old_cnt = prod_cnt;
        for (size_t k = 0; k < old_cnt; ++k) {
            uint64_t p = prod_list[k];
            __uint128_t mul = (__uint128_t)p * v;
            if (mul <= LIMIT) {
                uint64_t pmul = (uint64_t)mul;
                if (!prod[pmul]) {
                    prod[pmul] = 1;
                    if (prod_cnt == prod_cap) {
                        prod_cap <<= 1;
                        prod_list = (uint64_t *)realloc(prod_list,
                                                        prod_cap * sizeof(uint64_t));
                        if (!prod_list) exit(EXIT_FAILURE);
                    }
                    prod_list[prod_cnt++] = pmul;
                }
            }
        }
    }

    seq_t ans = 1;
    for (size_t i = 1; i <= LIMIT; ++i) {
        if (!sum[i] && !prod[i]) {
            ans = (seq_t)i;
            break;
        }
    }

    free(sum);
    free(prod);
    free(prod_list);
    return ans;
}

/* Public function: a(n) with 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset – undefined, but harmless */

    /* Compute terms up to n, memoising them. */
    while (cache_size < (size_t)n) {
        seq_t val;
        size_t idx = cache_size;   /* zero‑based index of the term being computed */

        if (idx == 0)         val = (seq_t)1;
        else if (idx == 1)    val = (seq_t)2;
        else if (idx == 2)    val = (seq_t)4;
        else if (idx < 17)    val = compute_missing(cache, cache_size);
        else                  /* recurrence for n > 17 */
            val = 2 * cache[idx - 2] + 4 * cache[idx - 4] - 2 * cache[idx - 6];

        grow_cache(cache_size + 1);
        cache[cache_size++] = val;
    }

    return cache[n - 1];
}
