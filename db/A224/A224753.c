/*
 * A224753  a(2)=2; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A224753:  a(1)=2; for n>1, a(n) is the smallest m such that
 *  a(n-1)+m = (a(n-1) followed by the leading digit of m).
 *  Equivalently, m = 9*a(n-1)+d where d is the leading digit of m. */

#include <stddef.h>                 /* for size_t */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Return the leading decimal digit of a positive x. */
static seq_t leading_digit(seq_t x)
{
    if (x == 0) return 0;               /* safety – never happens for x>0 */
    int len = num_digits(x);              /* number of decimal digits of x */
    seq_t p10 = pow10((idx_t)(len - 1)); /* 10^(len-1) */
    return x / p10;
}

/* Cached sequence. */
static seq_t *cache     = NULL;   /* array of values      */
static size_t cache_cap = 0;      /* allocated capacity   */
static size_t cache_len = 0;      /* number of terms stored */

/* Ensure that cache holds at least new_len elements. */
static void ensure_cache_size(size_t new_len)
{
    if (new_len <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < new_len) new_cap *= 2;

    if (cache == NULL) {
        cache = seq_alloc(new_cap);
    } else {
        cache = seq_realloc(cache, cache_cap, new_cap);
    }
    cache_cap = new_cap;
}

/* Compute a(n).  Cache previous values for fast repeated calls. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n<1 */

    size_t target = (size_t)n;      /* target index to compute */
    ensure_cache_size(target);

    if (cache_len == 0) {           /* initialise a(1) */
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* Extend cache up to target. */
    if (cache_len < target) {
        for (size_t i = cache_len; i < target; ++i) {
            seq_t prev = cache[i - 1];
            seq_t nine_prev = seq_mul(prev, (seq_t)9);   /* 9*a(n-1) */

            seq_t d = 0;
            for (int dd = 1; dd <= 9; ++dd) {
                seq_t cand = nine_prev + (seq_t)dd;     /* m = 9*prev + d */
                if (leading_digit(cand) == (seq_t)dd) {
                    d = (seq_t)dd;
                    break;
                }
            }
            cache[i] = nine_prev + d;  /* store a(n) */
        }
        cache_len = target;
    }

    return cache[n - 1];
}
