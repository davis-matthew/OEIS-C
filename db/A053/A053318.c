/*
 * A053318  a(n) contains n digits (either '2' or '7') and is divisible by
 *          2^n.
 * https://oeis.org/A053318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + 10^(n-1)*(2 + 5*((a(n-1)/2^(n-1)) mod 2)), with
 *            a(1) = 2 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 925 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* static cache for memoisation */
static seq_t *cache          = NULL;   /* indices 0 .. cache_cap-1 */
static size_t cache_cap      = 0;      /* allocated capacity   */
static size_t cache_max      = 0;      /* highest index computed */

/* a(n) --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* below offset 1: behaviour not required, return 0 */
    if (n == 0) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;                     /* indices 0 and 1 used */
        cache = seq_alloc_new_array(cache_cap);   /* allocate cache */
        cache[0] = (seq_t)0;               /* dummy entry for 0 */
        cache[1] = (seq_t)2;               /* a(1) = 2       */
        cache_max = 1;
    }

    /* grow cache if next index exceeds current capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* compute missing terms up to requested n */
    for (idx_t i = (idx_t)cache_max + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];                 /* a(i-1)          */
        seq_t pow2v  = pow2((idx_t)(i - 1));          /* 2^(i-1)         */
        seq_t q      = prev / pow2v;                  /* floor division  */
        seq_t odd    = q & (seq_t)1;                  /* q mod 2         */
        seq_t coeff  = (seq_t)2 + (seq_t)5 * odd;     /* 2 + 5*odd       */
        seq_t pow10v = pow10((idx_t)(i - 1));         /* 10^(i-1)        */
        cache[i] = prev + pow10v * coeff;             /* recurrence      */
    }

    cache_max = (size_t)n;
    return cache[n];
}
