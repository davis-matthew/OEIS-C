/*
 * A107433  Slowest increasing sequence where some digit of a(n) and some
 *          digit of a(n+1) add up to 9.
 * https://oeis.org/A107433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9898 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107433: a(0) = 0;
   for n > 0, a(n) is the smallest integer greater than a(n‑1) such that
   some digit of a(n‑1) and some digit of a(n) sum to 9. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether some digit of cur and some digit of cand sum to 9. */
static bool has_sum9(seq_t cur, seq_t cand)
{
    uint16_t maskCur = 0;
    seq_t t = cur;

    /* Build mask of digits present in cur. */
    if (t == 0) {
        maskCur |= 1U << 0;
    } else {
        while (t != 0) {
            int d = (int)(t % 10);
            maskCur |= 1U << d;
            t /= 10;
        }
    }

    /* Scan digits of cand looking for a complement in maskCur. */
    t = cand;
    bool ok = false;
    if (t == 0) {
        int d = 0;
        ok = (maskCur >> (9 - d)) & 1U;
    } else {
        while (t != 0 && !ok) {
            int d = (int)(t % 10);
            int comp = 9 - d;
            ok = (maskCur >> comp) & 1U;
            t /= 10;
        }
    }
    return ok;
}

/* Cache of previously computed terms. */
static seq_t *cache = NULL;
static idx_t cacheSize = 0;   /* number of stored terms */
static idx_t cacheCap  = 0;   /* capacity of the cache */

static void ensure_cache(idx_t newSize)
{
    if (newSize <= cacheCap)
        return;

    size_t newCap = cacheCap ? cacheCap : 16;
    while (newCap < (size_t)newSize)
        newCap <<= 1;

    seq_t *newCache = mem_resize(seq_t, cache, newCap);
    if (!newCache)   /* allocation failure – abort without modifying cache */
        return;

    cache = newCache;
    cacheCap = newCap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < cacheSize)
        return cache[n];

    ensure_cache(n + 1);

    for (idx_t i = cacheSize; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 0;
        } else {
            seq_t cur = cache[i - 1];
            seq_t cand = cur + (seq_t)1;
            while (!has_sum9(cur, cand))
                ++cand;
            value = cand;
        }
        cache[i] = value;
    }
    cacheSize = n + 1;
    return cache[n];
}
