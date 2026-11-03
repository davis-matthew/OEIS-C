/*
 * A069707  Squares with property that swapping first and last digits also
 *          gives a square.
 * https://oeis.org/A069707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 967 (2026-11-03)
 * Helpers:   ipow_i32, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth square such that swapping its first and last digits
   yields another square. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

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

static seq_t swap_first_last(seq_t n)
{
    int len = num_digits(n);
    if (len <= 1) return n;

    /* 10^(len-1) */
    seq_t pow10 = (seq_t)1;
    for (int i = 1; i < len; ++i)
        pow10 = seq_mul(pow10, (seq_t)10);

    seq_t d_first = n / pow10;
    seq_t d_last  = n % (seq_t)10;
    seq_t sub     = n % pow10;       /* lowest len-1 digits */
    seq_t mid     = sub / (seq_t)10; /* middle digits */

    seq_t y = seq_mul(d_last, pow10);
    y = seq_mul(mid, (seq_t)10) + y + d_first;
    return y;
}

/*  a(n)  –  nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0, cache_cap = 0;
    static idx_t   last_m = 0;     /* last m processed (m^2) */

    if (n <= cache_len) return cache[n-1];

    /* generate further terms until we have at least n */
    for (;;) {
        ++last_m;
        seq_t ms = (seq_t)last_m;
        seq_t s  = seq_mul(ms, ms);          /* s = m^2, already a square */

        seq_t y = swap_first_last(s);
        seq_t t = floor_sqrt(y);
        if (seq_mul(t, t) == y) {            /* y is a square too */

            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = s;

            if (cache_len >= n) return s;
        }
    }
}
