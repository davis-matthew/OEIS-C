/*
 * A063570  Smallest positive power of 7 having n in its decimal
 *          representation.
 * https://oeis.org/A063570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1013, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   multiply_by_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = smallest positive integer k such that the decimal
 * representation of n occurs in 7^k.
 * The sequence is computed on demand by repeatedly generating powers
 * 7^k as decimal strings and searching for the substring.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/multiply_by_7.h"

/* ---------- Globals for memoised powers of 7 and cache of results ---------- */
static char **pow_str = NULL;      /* array of decimal strings for 7^k */
static size_t pow_cnt = 0;         /* number of powers stored (pow_cnt = current max exponent) */

static seq_t *a_cache = NULL;     /* cache of a(n) values, zero means not yet computed */
static size_t cache_size = 0;      /* current size of a_cache */

/* ---------- Allocate or grow a_cache up to size new_size ---------- */
static void ensure_a_cache(size_t new_size)
{
    if (new_size <= cache_size)
        return;
    size_t new_cap = new_size + 1; /* allow zero index */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    /* Initialise new slots to 0 */
    for (size_t i = cache_size; i < new_cap; ++i)
        a_cache[i] = 0;
    cache_size = new_cap;
}

/* ---------- Ensure powers of 7 are computed up to exponent 'k' (1‑based) ---------- */
static void ensure_pow_str(size_t k)
{
    if (pow_cnt >= k)
        return;
    /* Initialize first power if needed */
    if (pow_cnt == 0) {
        pow_str = (char **)malloc(sizeof(char *));
        pow_str[0] = strdup("7"); /* 7^1 */
        pow_cnt = 1;
    }
    /* Grow array as needed */
    while (pow_cnt < k) {
        pow_str = (char **)realloc(pow_str, (pow_cnt + 1) * sizeof(char *));
        pow_str[pow_cnt] = NULL;
        multiply_by_7(pow_str[pow_cnt - 1], &pow_str[pow_cnt]); /* 7^(pow_cnt+1) */
        pow_cnt++;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Return cached result if available */
    if (n < cache_size && a_cache[n] != 0)
        return a_cache[n];

    /* Convert n to decimal string */
    char n_str[32];
    snprintf(n_str, sizeof(n_str), "%llu", (unsigned long long)n);

    /* Scan powers of 7 until n appears as substring */
    for (size_t k = 1; ; ++k) {
        ensure_pow_str(k);   /* make sure 7^k is available */
        if (strstr(pow_str[k - 1], n_str) != NULL) {
            seq_t res = (seq_t)k;
            /* Store in cache for future calls */
            if (n < cache_size)
                a_cache[n] = res;
            return res;
        }
    }
}
