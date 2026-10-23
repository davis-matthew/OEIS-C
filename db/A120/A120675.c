/*
 * A120675  Number of prime factors of odd squarefree numbers A056911.
 * https://oeis.org/A120675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors of the nth odd squarefree number
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static idx_t cache_size = 0;       /* number of computed terms (1‑based) */
static idx_t limit = 0;            /* current maximum number sieved */
static seq_t *cache = NULL;        /* cache[n] = a(n) */
static uint32_t *spf = NULL;       /* smallest prime factor for numbers ≤ limit */

/* rebuilds spf and cache up to new_limit */
static void rebuild(idx_t new_limit)
{
    /* free old spf */
    if (spf != NULL) free(spf);
    spf = (uint32_t *)malloc((size_t)(new_limit + 1) * sizeof(uint32_t));
    if (spf == NULL) abort();  /* allocation failed */

    /* initialise spf[i] = i */
    for (idx_t i = 0; i <= new_limit; ++i) spf[i] = (uint32_t)i;

    /* simple sieve to fill spf */
    for (idx_t i = 2; i * i <= new_limit; ++i) {
        if (spf[i] == (uint32_t)i) {            /* i is prime */
            for (idx_t j = i * i; j <= new_limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* rebuild cache: collect odd squarefree numbers up to new_limit */
    seq_t *new_cache = (seq_t *)calloc((size_t)new_limit + 1, sizeof(seq_t));
    if (new_cache == NULL) abort();

    idx_t cnt = 0;
    for (idx_t num = 1; num <= new_limit; num += 2) {   /* odd numbers only */
        idx_t x = num;
        idx_t factor_cnt = 0;
        uint32_t prev_prime = 0;
        bool squarefree = true;

        while (x > 1) {
            uint32_t p = spf[x];
            if (p == prev_prime) {          /* repeated prime factor */
                squarefree = false;
                break;
            }
            prev_prime = p;
            ++factor_cnt;
            x /= p;
        }

        if (squarefree) {
            ++cnt;
            new_cache[cnt] = (seq_t)factor_cnt;
        }
    }

    /* replace old cache */
    free(cache);
    cache = new_cache;
    cache_size = cnt;
    limit = new_limit;
}

/* ensure that cache contains at least n terms */
static void ensure_cache(idx_t n_needed)
{
    if (n_needed <= cache_size) return;

    idx_t new_limit = (limit == 0) ? 1 : limit * 2;

    /* keep doubling until enough terms are available */
    while (true) {
        rebuild(new_limit);
        if (cache_size >= n_needed) break;
        new_limit *= 2;
    }
}

/* sequence function requested by OEIS */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but return 0 */
    ensure_cache(n);
    return cache[n];
}
