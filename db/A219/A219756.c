/*
 * A219756  Expansion of
 *          x^4*(1-7*x+17*x^2-18*x^3+11*x^4-5*x^5)/((1-x)^2*(1-3*x)^2*(1-3*x+x^2)^2).
 * https://oeis.org/A219756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 81*a(n-2) + 250*a(n-3) - 444*a(n-4) +
 *            458*a(n-5) - 265*a(n-6) + 78*a(n-7) - 9*a(n-8), for n ≥ 10
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=7,
 *            a(6)=34, a(7)=141, a(8)=537, a(9)=1942
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;   /* number of terms stored */

static const seq_t init_vals[10] = {
    /* 0..9 */
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)7, (seq_t)34, (seq_t)141,
    (seq_t)537, (seq_t)1942
};

static void ensure_cache(idx_t n)
{
    /* Make sure that seq_cache holds at least the first 10 terms. */
    if (cache_size < 10) {
        size_t needed = 10;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = cache_size; i < 10; ++i)
            seq_cache[i] = init_vals[i];
        if (cache_size < 10)
            cache_size = 10;
    }

    /* If the requested index is already cached, nothing more to do. */
    if ((size_t)n < cache_size) return;

    /* Grow the capacity if needed. */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* For indices 0..9 we already have correct values. */
        if (i < 10) continue;

        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;
        pos += (seq_t)14 * seq_cache[i-1];
        pos += (seq_t)250 * seq_cache[i-3];
        pos += (seq_t)458 * seq_cache[i-5];
        pos += (seq_t)78  * seq_cache[i-7];

        neg += (seq_t)81  * seq_cache[i-2];
        neg += (seq_t)444 * seq_cache[i-4];
        neg += (seq_t)265 * seq_cache[i-6];
        neg += (seq_t)9   * seq_cache[i-8];

        seq_cache[i] = pos - neg;   /* pos ≥ neg, safe for unsigned */
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[(size_t)n];
}
