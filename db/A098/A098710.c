/*
 * A098710  a(n) = Product_{k|n} k!.
 * https://oeis.org/A098710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} d!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Need the factorization helper */
#include "../utils/factor_u64.h"
#include <stdlib.h>

/* ---------- factorial cache ---------- */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  */
static idx_t fact_cache_n = 0;     /* highest i cached */

static void ensure_fact(idx_t n)
{
    if (n <= fact_cache_n)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!fact_cache) {
        new_cache[0] = (seq_t)1;
    } else {
        for (idx_t i = 0; i <= fact_cache_n; ++i)
            new_cache[i] = fact_cache[i];
    }
    for (idx_t i = fact_cache_n + 1; i <= n; ++i)
        new_cache[i] = new_cache[i - 1] * (seq_t)i;

    if (fact_cache)
        free(fact_cache);
    fact_cache = new_cache;
    fact_cache_n = n;
}

/* ---------- a(n) cache ---------- */
static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)  */
static idx_t a_cache_n = 0;     /* highest i cached */

static void ensure_a_cache(idx_t n)
{
    if (n <= a_cache_n)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (a_cache) {
        for (idx_t i = 0; i <= a_cache_n; ++i)
            new_cache[i] = a_cache[i];
        free(a_cache);
    }
    /* uninitialized entries are fine; we will set them upon first use */
    a_cache = new_cache;
    a_cache_n = n;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined; 0 is a safe fallback */

    if (a_cache && n <= a_cache_n)
        return a_cache[n];

    /* ---------- factor n ---------- */
    uint64_t pr[32], exp[32];
    size_t pcnt;
    factor_u64((uint64_t)n, pr, exp, &pcnt);

    /* ---------- enumerate all divisors ---------- */
    /* We first allocate a divisors array; divisors count is small
       for n <= 10000 (at most a few hundred). */
    size_t divs_cap = 64;
    uint64_t *divs = (uint64_t *)malloc(divs_cap * sizeof(uint64_t));
    size_t divs_len = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i];
        unsigned e = (unsigned)exp[i];
        size_t old_len = divs_len;
        for (size_t j = 0; j < old_len; ++j) {
            uint64_t base = divs[j];
            uint64_t val = base;
            for (unsigned k = 1; k <= e; ++k) {
                val *= p;
                if (divs_len == divs_cap) {
                    divs_cap *= 2;
                    divs = (uint64_t *)realloc(divs, divs_cap * sizeof(uint64_t));
                }
                divs[divs_len++] = val;
            }
        }
    }

    /* ---------- multiply factorials of all divisors ---------- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < divs_len; ++i) {
        idx_t d = (idx_t)divs[i];
        ensure_fact(d);
        result *= fact_cache[d];
    }
    free(divs);

    /* ---------- cache the result ---------- */
    ensure_a_cache(n);
    a_cache[n] = result;
    return result;
}
