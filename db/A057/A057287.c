/*
 * A057287  Numbers n such that n | 10^n + 9^n + 8^n + 7^n + 6^n + 5^n + 4^n +
 *          3^n + 2^n.
 * https://oeis.org/A057287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute numbers n such that n | 10^n + 9^n + … + 2^n.  This
   function returns the nth such number (OEIS a(n), offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* add value to *sum modulo mod   (no overflow beyond 2*mod) */
static inline void add_modulo(seq_t *sum, seq_t value, seq_t mod)
{
    *sum += value;
    if (*sum >= mod)
        *sum -= mod;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t max_checked = 0;   /* last integer that was tested */

/* test whether m divides sum_{b=2}^{10} b^m */
static bool good_candidate(idx_t m)
{
    seq_t sum   = 0;
    seq_t mod   = (seq_t)m;
    for (int base = 2; base <= 10; ++base) {
        seq_t p = powmod((seq_t)base, (uint64_t)m, mod);
        add_modulo(&sum, p, mod);
    }
    return sum == 0;
}

/* a(n) – nth element of the sequence, n >= 1; undefined for n == 0 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety for offset‑1 callers */

    /* grow the cache until it has at least n elements */
    while (cache_len < n) {
        ++max_checked;
        if (good_candidate(max_checked)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)max_checked;
        }
    }
    return cache[n - 1];
}
