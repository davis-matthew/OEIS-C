/*
 * A067490  Powers of 4 with initial digit 1.
 * https://oeis.org/A067490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 363 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A067490 sequence: powers of 4 whose decimal representation
   starts with the digit 1.  a(1) = 1, a(2) = 16, a(3) = 1024, ... */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static void ensure_cache_capacity(idx_t need,
                                  seq_t **cache, size_t *cap)
{
    size_t required = (size_t)need;
    if (*cache == NULL) {
        *cap = required > 8 ? required * 2 : 8;
        *cache = seq_alloc(*cap);
    } else if (*cap < required) {
        size_t new_cap = *cap * 2;
        if (new_cap < required)
            new_cap = required;
        seq_t *tmp = seq_realloc(*cache, *cap, new_cap);
        if (tmp == NULL)
            return;  /* allocation failed – caller will see incomplete cache */
        *cache = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* Undefined for n < 1 */

    /* Static cache of observed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current exponent of the running power 4^k and its value */
    static idx_t current_exp = 0;
    static seq_t current_power = (seq_t)1;   /* 4^0 */

    /* Initialise cache on first call */
    if (cache_len == 0) {
        ensure_cache_capacity(1, &cache, &cache_cap);
        cache[0] = current_power;  /* 4^0 = 1 */
        cache_len = 1;
    }

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* Advance to next exponent */
        current_exp++;
        current_power = mul_small(current_power, (unsigned int)4);

        /* Determine if 4^current_exp starts with digit 1 */
        size_t len = num_digits(current_power);
        seq_t divisor = pow10((idx_t)(len - 1));   /* 10^(len-1) */
        seq_t high_digit = current_power / divisor;

        if (high_digit == (seq_t)1) {
            /* Store this term */
            ensure_cache_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len] = current_power;
            cache_len++;
        }
    }

    return cache[n - 1];
}
