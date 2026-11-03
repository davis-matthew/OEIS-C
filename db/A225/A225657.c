/*
 * A225657  Union of {6} and A000961.
 * https://oeis.org/A225657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth term of the sorted union of {6} and all prime powers
 *            (including 1)
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* dynamic array helper that can reallocate the array itself */
static void add_value(seq_t **arr, size_t *len, size_t *cap, seq_t v)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 256 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = v;
    (*len)++;
}

/* generate all prime powers <= limit (including 1) and store in *vals */
static void generate_prime_powers(idx_t limit, seq_t **vals, size_t *len, size_t *cap)
{
    /* Sieve of Eratosthenes for primes up to limit */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes into a temporary array */
    uint32_t *primes = (uint32_t *)malloc((size_t)(limit / 10 + 1) * sizeof(uint32_t));
    size_t primes_cap = limit / 10 + 1;
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            if (primes_cnt >= primes_cap) {
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(primes); free(is_prime); exit(EXIT_FAILURE); }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* add 1 first */
    add_value(vals, len, cap, (seq_t)1);

    seq_t lim = (seq_t)limit;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        seq_t p = (seq_t)primes[pi];
        /* add the prime itself and all higher powers */
        seq_t val = p;
        while (true) {
            add_value(vals, len, cap, val);
            if (val > lim / p) break;
            val *= p;
        }
    }

    /* add the exceptional element 6 */
    add_value(vals, len, cap, (seq_t)6);

    free(primes);
    free(is_prime);
}

/* comparison function for qsort on seq_t values */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *v1 = (const seq_t *)p1;
    const seq_t *v2 = (const seq_t *)p2;
    if (*v1 < *v2) return -1;
    if (*v1 > *v2) return 1;
    return 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static idx_t   cache_lim = 0;   /* sieve limit we have generated so far */

    /* ensure we have at least n terms cached */
    while (cache_len < n) {
        /* double the sieve limit until the cache covers n */
        idx_t new_lim = (cache_lim == 0) ? 1024 : cache_lim * 2;
        /* regenerate cache */
        seq_t *vals = NULL;
        size_t vals_len = 0, vals_cap = 0;
        generate_prime_powers(new_lim, &vals, &vals_len, &vals_cap);

        /* sort and deduplicate */
        mem_qsort(vals, vals_len, sizeof(seq_t), cmp_seq);
        size_t uniq = 0;
        for (size_t i = 0; i < vals_len; ++i) {
            if (uniq == 0 || vals[i] != vals[uniq - 1]) {
                vals[uniq++] = vals[i];
            }
        }
        vals_len = uniq;

        /* free previous cache */
        mem_free(cache);
        cache = vals;
        cache_len = vals_len;
        cache_lim = new_lim;

        /* safety: if after regeneration we still don't have enough terms, increase limit */
        if (cache_len < n) {
            /* increase limit further */
            continue;
        }
    }

    /* return the (n‑1)th element (1‑based indexing) */
    return cache[n - 1];
}
