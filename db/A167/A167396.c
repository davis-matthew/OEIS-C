/*
 * A167396  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 15, a(n) = 32·∑_{i=1}^{13} a(n-i) – 528·a(n-14);
 *            Initial values (n = 0 … 14) are: 1, 34, 1122, 37026, 1221858,
 *            40321314, 1330603362, 43909910946, 1449027061218,
 *            47817893020194, 1577990469666402, 52073685498991266,
 *            1718431621466711778, 56708243508401488674,
 *            1871372035777249125681 These va...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

#define PRE_LEN 15  /* number of pre‑computed terms (indices 0 … 14) */

static seq_t *pre = NULL;       /* cache of the first 15 terms          */
static size_t pre_cap = 0;
static idx_t pre_len = 0;

static seq_t *cache = NULL;     /* cache for arbitrary indices          */
static size_t cache_cap = 0;
static idx_t cache_len = 0;

/* Ensure that the pre array contains the first 15 terms. */
static void ensure_pre(void)
{
    if (pre)
        return;

    static const char *init_str[PRE_LEN] = {
        "1",
        "34",
        "1122",
        "37026",
        "1221858",
        "40321314",
        "1330603362",
        "43909910946",
        "1449027061218",
        "47817893020194",
        "1577990469666402",
        "52073685498991266",
        "1718431621466711778",
        "56708243508401488674",
        "1871372035777249125681"
    };

    pre_cap = PRE_LEN;
    pre_len = PRE_LEN;
    pre = (seq_t *)malloc(pre_cap * sizeof(seq_t));
    for (idx_t i = 0; i < PRE_LEN; ++i) {
        pre[i] = seq_from_str(init_str[i]);
    }
}

/* Grow the cache array so that it can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1))
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)n) {
        new_cap <<= 1;           /* double until big enough */
    }

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_arr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Compute the nth term of sequence A167396. */
extern seq_t a(idx_t n)
{
    if (n < PRE_LEN) {
        ensure_pre();
        return pre[n];
    }

    ensure_cache(n);

    /* If cache already contains the answer, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Initialise the cache with the precomputed initial terms if needed. */
    if (cache_len == 0) {
        ensure_pre();
        /* Ensure cache is large enough to hold the initial terms. */
        if (cache_cap < PRE_LEN) {
            size_t new_cap = PRE_LEN;
            seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
            memcpy(new_arr, pre, PRE_LEN * sizeof(seq_t));
            free(cache);
            cache = new_arr;
            cache_cap = new_cap;
        }
        for (idx_t i = 0; i < PRE_LEN; ++i) {
            cache[i] = pre[i];
        }
        cache_len = PRE_LEN;
    }

    /* Compute successive terms using the linear recurrence. */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 1; j <= 13; ++j) {
            sum += cache[k - j];
        }
        seq_t term = sum * 32;
        term -= cache[k - 14] * (seq_t)528;
        cache[k] = term;
    }
    cache_len = n + 1;
    return cache[n];
}
