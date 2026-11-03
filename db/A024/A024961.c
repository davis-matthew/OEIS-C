/*
 * A024961  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with a(1)=2,
 *          a(2)=1, and a(3)=3.
 * https://oeis.org/A024961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), a(1)=2,
 *            a(2)=1, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated array size (indices 0..cache_cap-1) */
static idx_t  cache_last = 0;  /* largest n for which cache[n] is valid */

/* Return the nth term of sequence A024961. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialise the cache on the first call or if the requested n is
       beyond the current capacity */
    if (cache == NULL) {
        /* ensure capacity for at least n+1 indices (since we ignore cache[0]) */
        cache_cap = (size_t)n + 1;
        if (cache_cap < 4) cache_cap = 4;            /* room for a(1..3) */
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;                       /* allocation failed */

        /* initial values */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache_last = 3;
    }

    /* grow the cache if necessary */
    if ((idx_t)(cache_cap - 1) < n) {
        size_t new_cap = cache_cap * 2;
        while ((idx_t)(new_cap - 1) < n) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;                     /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill in missing values up to n */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        if (k < 4) {                            /* predefined terms */
            switch (k) {
                case 1: cache[k] = (seq_t)2; break;
                case 2: cache[k] = (seq_t)1; break;
                case 3: cache[k] = (seq_t)3; break;
            }
            continue;
        }

        idx_t limit = (k + 2) / 4;              /* floor((n+2)/4) */
        seq_t sum = 0;
        for (idx_t i = 1; i <= limit; ++i) {
            seq_t left  = cache[2 * i - 1];
            seq_t right = cache[k - 2 * i + 1];
            sum += seq_mul(left, right);
        }
        cache[k] = sum;
    }

    cache_last = n;
    return cache[n];
}
