/*
 * A080446  a(1) = 2, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1).
 * https://oeis.org/A080446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=2; for n>1, a(n)=smallest k*a(n-1) with k>=2, k*a(n-1) has
 *            exactly n decimal digits, and k*a(n-1)%10!=0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
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

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

/* Static cache of computed terms.  cache[1] is a(1). */
static seq_t *cache     = NULL;   /* array of terms, index 0 unused */
static size_t cache_cap = 0;      /* allocated size of cache array */
static idx_t  cache_len = 0;      /* highest term stored (>=1) */

seq_t a(idx_t n)
{
    if (n < 1) {                /* undefined for non‑positive indices */
        return (seq_t)0;
    }

    /* Ensure cache has room for index n.  Allocate or grow dynamically. */
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = (n + 1) * 2;
        cache = seq_alloc_new_array(cache_cap);
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    /* Initialize first term if necessary. */
    if (cache_len < 1) {
        cache[1] = seq_from_str("2");
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        seq_t low    = pow10(i - 1);   /* 10^(i-1)  -- lower bound (inclusive) */
        seq_t high   = pow10(i);       /* 10^i      -- upper bound (exclusive) */
        seq_t m      = (low + prev - 1) / prev;   /* ceil(low / prev)  */
        seq_t prod;

        for (;;) {
            prod = prev * m;          /* candidate multiple */
            if (prod >= low && prod < high && (prod % (seq_t)10) != 0) {
                break;                /* valid term found */
            }
            ++m;
        }

        cache[i] = prod;
    }

    cache_len = n;
    return cache[n];
}
