/*
 * A023887  a(n) = sigma_n(n): sum of n-th powers of divisors of n.
 * https://oeis.org/A023887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} d^n (OEIS A023887)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* utilities used */
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* cache for memoisation */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

/* ensure cache has room for index n */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_cap)
        return;

    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);

    if (a_cache) {
        memcpy(new_cache, a_cache, a_cache_cap * sizeof(seq_t));
        free(a_cache);
    }
    memset(new_cache + a_cache_cap, 0, (new_cap - a_cache_cap) * sizeof(seq_t));

    a_cache      = new_cache;
    a_cache_cap  = new_cap;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1; undefined for 0 */
        return 0;

    ensure_cache(n);

    if (a_cache[n] != 0)    /* already computed */
        return a_cache[n];

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* generate all divisors */
    size_t div_cap = 1;
    size_t div_cnt = 1;
    uint64_t *divs = (uint64_t *)malloc(div_cap * sizeof(uint64_t));
    if (!divs)
        exit(1);
    divs[0] = 1ULL;           /* start with divisor 1 */

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        size_t old_cnt = div_cnt;

        for (size_t j = 0; j < old_cnt; ++j) {
            uint64_t base = divs[j];
            uint64_t val  = base;
            for (uint64_t k = 1; k <= e; ++k) {
                val *= p;
                if (div_cnt + 1 > div_cap) {
                    div_cap *= 2;
                    divs = (uint64_t *)realloc(divs, div_cap * sizeof(uint64_t));
                    if (!divs) exit(1);
                }
                divs[div_cnt++] = val;
            }
        }
    }

    /* sum d^n over all divisors */
    seq_t sum = 0;
    seq_t exp_n = (seq_t)n;

    for (size_t i = 0; i < div_cnt; ++i)
        sum += big_pow((seq_t)divs[i], exp_n);

    free(divs);

    a_cache[n] = sum;
    return sum;
}
