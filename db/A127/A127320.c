/*
 * A127320  Start with i=1 and j=2. Concatenate i and j, get k = floor(ij/j),
 *          concatenate j and k, etc.
 * https://oeis.org/A127320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_len, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A127320:  
 *   a(1)=1, a(2)=2; for n≥3,  
 *   a(n)=⌊concat(a(n-2),a(n-1))/a(n-1)⌋  
 * where concat(x,y) is the decimal concatenation of the two
 * non‑negative integers.
 *
 * Implementation uses a simple memoised generator.  All arithmetic
 * is performed in seq_t; no 128‑bit temporaries are used.
 * The routine is thread‑unsafe but satisfies the required API.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>          /* memcpy */

#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/digit_len.h"   /* int digit_len(seq_t x) */

static seq_t *cache = NULL;      /* 1‑based array of terms */
static size_t cache_cap = 0;     /* allocated size (indices 0..cache_cap) */
static idx_t  computed   = 0;    /* last computed index */

/* Grow the cache if necessary and compute terms up to n. */
static void ensure_size(idx_t n)
{
    if (n <= computed) return;

    /* Allocate bigger array if needed. */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;        /* we store indices 0..n inclusive */
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();        /* out of memory */
        if (cache) {
            memcpy(new_arr, cache, (computed + 1) * sizeof(seq_t));
            free(cache);
        }
        cache     = new_arr;
        cache_cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        if (idx == 1) {
            cache[1] = (seq_t)1;
        } else if (idx == 2) {
            cache[2] = (seq_t)2;
        } else {
            seq_t i = cache[idx - 2];
            seq_t j = cache[idx - 1];

            seq_t pow10 = pow_uint(10, digit_len(j));
            seq_t concat = i * pow10 + j;
            cache[idx]   = concat / j;   /* floor division on non‑negative */
        }
    }
    computed = n;
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but guard against 0 */
    ensure_size(n);
    return cache[n];
}
