/*
 * A044960  Numbers with no two equally numerous base 11 digits.
 * https://oeis.org/A044960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044960
   Numbers whose base‑11 digit counts are all distinct.
   The sequence is defined by a direct search: a(n) is the nth integer
   for which, when written in base 11, no two appearing digits occur the
   same number of times.
   Implementation details:
   * A static cache stores previously found terms.
   * Each candidate number is tested by examining its base‑11 digits.
   * The cache grows by reallocating a larger array via the provided
     seq_alloc/seq_free utilities.
   * The function works for any seq_t/idx_t provided by the build. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static void seq_free(seq_t *p) { mem_free(p); }

/* Test whether a value satisfies the unique‑digit‑count property. */
static bool has_unique_digit_counts(seq_t v)
{
    /* Count occurrences of each base‑11 digit. */
    unsigned char cnt[11] = {0};
    seq_t tmp = v;
    while (tmp > 0) {
        int d = (int)(tmp % 11);
        cnt[d]++;
        tmp /= 11;
    }

    /* Check that the frequencies of appearing digits are all distinct. */
    unsigned char seen[256] = {0};
    for (int i = 0; i < 11; ++i) {
        unsigned char c = cnt[i];
        if (c > 0) {
            if (seen[c])
                return false;
            seen[c] = 1;
        }
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* caller should not request n==0 */

    /* Static cache for previously computed terms. */
    static seq_t *cache    = NULL;   /* array of terms */
    static size_t cache_len = 0;     /* number of terms stored     */
    static size_t cache_cap = 0;     /* allocated capacity         */
    static seq_t next_val = (seq_t)1;/* next candidate to test     */

    /* Grow the cache until it contains the nth term. */
    while (cache_len < n) {
        if (has_unique_digit_counts(next_val)) {
            /* Allocate or grow the cache array. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = seq_alloc(new_cap);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                if (cache)
                    seq_free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        ++next_val;
    }

    return cache[n - 1];
}
