/*
 * A041512  Numerators of continued fraction convergents to sqrt(273).
 * https://oeis.org/A041512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=16; a(1)=17; for n>=2,
 * a(n)=P(n)*a(n-1)+a(n-2), where P(n) is the (n‑1)‑th term of
 * the periodic part of the continued‑fraction expansion of sqrt(273). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static unsigned period[64];
static size_t period_len = 0;

/* Compute the periodic part of sqrt(273)'s continued fraction. */
static void generate_period(void)
{
    const uint64_t N = 273;
    unsigned a0 = 0;
    while (((uint64_t)(a0 + 1) * (a0 + 1)) <= N)
        a0++;
    uint64_t m = 0, d = 1;
    unsigned a = a0;
    period_len = 0;
    while (1) {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
        period[period_len++] = a;
        if (a == 2 * a0)
            break;
    }
}

/* Expand the cache to include terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len)
        return;

    if (period_len == 0)
        generate_period();

    size_t new_len = (size_t)n + 1;
    if (new_len > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < new_len)
            new_cap = new_len;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache_len == 0) {
        cache[0] = (seq_t)16;   /* a(0) */
        cache_len = 1;
    }

    while (cache_len <= (size_t)n) {
        if (cache_len == 1) {            /* a(1) = a1 * a0 + 1 */
            unsigned a1 = period[0];
            cache[1] = (seq_t)a1 * cache[0] + (seq_t)1;
        } else {
            unsigned a_i = period[(cache_len - 1) % period_len];
            cache[cache_len] = (seq_t)a_i * cache[cache_len - 1]
                               + cache[cache_len - 2];
        }
        cache_len++;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
