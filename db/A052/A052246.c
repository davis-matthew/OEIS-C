/*
 * A052246  Concatenation of integers from n down to 0.
 * https://oeis.org/A052246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*10^len(a(n-1)) + a(n-1) with a(0)=0. Computation is
 *            iterative with a cache for out‑of‑order calls. Only integer
 *            operators and constant arithmetic are used
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   digit_len_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_len_seq_t.h"

/* External types supplied by the build. */
#ifndef idx_t
#define idx_t uint64_t
#endif
#ifndef seq_t
/* The build defines seq_t for the target integer type. */
#endif

/* Count decimal digits of a non‑negative idx_t (fast for small n). */
static size_t digit_count_idx(idx_t v) {
    size_t d = 0;
    do {
        v /= 10;
        ++d;
    } while (v);
    return d;
}

/* 10^exp for small exp (exp≤20).  Using seq_t arithmetic. */
static seq_t pow10_small(int exp) {
    seq_t r = 1;
    for (int i = 0; i < exp; ++i) r *= 10;
    return r;
}

/* Main sequence function.  Keeps a static cache. */
seq_t a(idx_t n) {
    /* Cache for already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached elements */

    if (n >= cache_size) {
        /* Grow cache to hold indices up to n. */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* out of memory – not expected */
        cache = tmp;

        /* Determine starting state for the new segment. */
        size_t start = cache_size;
        size_t digits_prev; /* len(a(start-1)) */
        seq_t shift;        /* 10^digits_prev */

        if (start == 0) {
            /* No previous terms; a(0)=0. */
            digits_prev = 1;
            shift = pow10_small(1);   /* 10 */
        } else {
            /* Get digits of the last cached term. */
            digits_prev = digit_len_seq_t(cache[start - 1]);
            shift = pow10_small((int)digits_prev);
        }

        /* Compute new terms. */
        for (size_t i = start; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
                continue;
            }
            /* shift currently equals 10^len(a(i-1)) */
            seq_t val = (seq_t)i * shift + cache[i - 1];
            cache[i] = val;

            /* Update shift and digits for next iteration. */
            size_t digits_i = digit_count_idx((idx_t)i);     /* len(i) */
            seq_t pow10d = pow10_small((int)digits_i);       /* 10^len(i) */
            shift *= pow10d;                                 /* 10^(len(a(i))) */
            digits_prev += digits_i;
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
