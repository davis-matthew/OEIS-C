/*
 * A126957  Numbers n with all digits distinct, no 0 or 1 digits and such that
 *          n is divisible by the product of its digits.
 * https://oeis.org/A126957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code implements a search for OEIS A126957:
   numbers with all digits distinct, no 0 or 1 digits,
   and divisible by the product of their digits.
   The algorithm enumerates all such 1‑ to 8‑digit numbers
   (digits 2‑9), tests each, stores the valid ones,
   sorts them, and then returns the nth element on request. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Forward declarations */
static void generate(seq_t curr, uint16_t usedMask, uint64_t prod, uint8_t len);
static void add_value(seq_t val);
static int cmp_seq(const void *a, const void *b);

/* Internal cache */
static seq_t *cache      = NULL;
static size_t   cache_cap= 0;
static size_t   cache_len= 0;
static bool     cache_init = false;

/* Public function */
seq_t a(idx_t n)
{
    if (!cache_init) {                      /* first call, build cache */
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        generate(0, 0, 1, 0);               /* build all candidates */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
        cache_init = true;
    }
    if (n == 0 || (idx_t)n > cache_len) return 0; /* out of bounds */
    return cache[(size_t)(n-1)];
}

/* Recursively build numbers with distinct digits 2‑9 */
static void generate(seq_t curr, uint16_t usedMask, uint64_t prod, uint8_t len)
{
    if (len > 0) {                       /* we have a complete number */
        if (prod != 0 && curr % prod == 0)
            add_value(curr);             /* keep valid ones */
    }
    if (len == 8) return;                /* max 8 digits (2‑9) */

    for (uint8_t d = 2; d <= 9; ++d) {    /* try each unused digit */
        uint16_t bit = 1u << d;
        if (usedMask & bit) continue;
        generate(curr*10 + d,
                 usedMask | bit,
                 prod * d,
                 len + 1);
    }
}

/* Append a value to the cache, resizing if necessary */
static void add_value(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 128;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Comparator for qsort – works with any unsigned integer or big‑int type */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t*)a;
    const seq_t *y = (const seq_t*)b;
    return (*x > *y) - (*x < *y);
}
