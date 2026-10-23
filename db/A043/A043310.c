/*
 * A043310  a(n)=A033004(n)/7.
 * https://oeis.org/A043310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Definition: a(n) is the nth positive integer whose base-36 representation uses only the digits 0..5 and has no consecutive equal digits.
 * The function enumerates all such numbers in increasing numeric order.
 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* array holding generated terms */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* current allocated capacity */
static idx_t generated_max_length = 0;  /* longest length of numbers generated */

/* recursive generator for a fixed length L */
static void gen_rec(idx_t pos, idx_t L, int prev, seq_t val) {
    if (pos == L) {
        /* ensure capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
        return;
    }
    if (pos == 0) {                      /* leading digit cannot be 0 */
        for (int d = 1; d <= 5; ++d) {
            seq_t new_val = val * (seq_t)36 + (seq_t)d;
            gen_rec(pos + 1, L, d, new_val);
        }
    } else {                             /* subsequent digits, no repetition */
        for (int d = 0; d <= 5; ++d) {
            if (d == prev) continue;
            seq_t new_val = val * (seq_t)36 + (seq_t)d;
            gen_rec(pos + 1, L, d, new_val);
        }
    }
}

/* generate all numbers of length L and append to cache */
static void gen_length(idx_t L) {
    gen_rec(0, L, -1, (seq_t)0);
}

/* compute sufficient precomputation up to n */
static void precompute(idx_t n) {
    if (cache == NULL) {                 /* first call: allocate buffer */
        cache_cap = 20000;               /* ample for n up to 10^4 */
        cache = alloc_seq_array(cache_cap);
    }
    if (cache_len >= n) return;          /* already have enough */

    /* find minimal length needed to reach n terms */
    idx_t target_len = 1;
    idx_t cumulative = 0;
    idx_t pow5 = 5;                      /* 5^1 */
    cumulative = pow5;
    while (cumulative < n) {
        ++target_len;
        pow5 *= 5;
        cumulative += pow5;
    }

    /* generate new lengths as needed */
    for (idx_t L = generated_max_length + 1; L <= target_len; ++L) {
        gen_length(L);
    }
    generated_max_length = target_len;
}

/* required function for the problem */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined for 0, return 0 */
    precompute(n);
    return cache[n - 1];
}
