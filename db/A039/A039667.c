/*
 * A039667  Numbers k that together with their double and triple contain every
 *          digit from 1-9 exactly once.
 * https://oeis.org/A039667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A039667
 *  Numbers k such that the concatenation of k, 2k and 3k contains each
 *  digit 1–9 exactly once and no zero.
 *  The function builds the sequence on demand, caching already found
 *  terms to allow out‑of‑order queries.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Count the decimal digits of v into cnt[0..9].  v is assumed non‑zero. */
static inline void count_digits(seq_t v, unsigned char *cnt)
{
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        ++cnt[d];
        v /= 10;
    }
}

/* Return true if a,b,c together contain each digit 1–9 exactly once
 * and contain no 0. */
static inline bool is_valid(seq_t a, seq_t b, seq_t c)
{
    unsigned char cnt[10] = {0};
    count_digits(a, cnt);
    count_digits(b, cnt);
    count_digits(c, cnt);
    if (cnt[0] != 0)  return false;
    for (int d = 1; d <= 9; ++d)
        if (cnt[d] != 1) return false;
    return true;
}

/* Grow the cache array to at least 'needed' elements. */
static void grow_cache(size_t needed, seq_t **cache, size_t *cap)
{
    if (*cap == 0) {
        *cap = 16;
    } else {
        while (*cap < needed) {
            *cap <<= 1;
            if (*cap == 0) *cap = 16; /* guard for overflow */
        }
    }
    seq_t *tmp = (seq_t *)realloc(*cache, *cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – terminate. */
        abort();
    }
    *cache = tmp;
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Static cache to keep previously found terms and the last
       candidate tested.  This allows out‑of‑order queries. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;

    while (cache_len <= (size_t)n) {
        idx_t k = last_k + 1;
        for (;;) {
            seq_t kval  = (seq_t)k;
            seq_t kval2 = kval * (seq_t)2U;
            seq_t kval3 = kval * (seq_t)3U;
            if (is_valid(kval, kval2, kval3)) {
                if (cache_len == cache_cap) {
                    grow_cache(cache_len + 1, &cache, &cache_cap);
                }
                cache[cache_len++] = kval;
                last_k = k;
                break;
            }
            ++k;
        }
    }
    return cache[(size_t)n];
}
