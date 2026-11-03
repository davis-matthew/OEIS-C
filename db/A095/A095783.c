/*
 * A095783  Values of n for which A095777(n) is 14 (those terms which are
 *          expressible in decimal digits for bases 2 through 15, but not for
 *          base 16).
 * https://oeis.org/A095783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A095783: a(n) is the n‑th integer k such that
 *   • in every base b = 2,3,…,15 the base‑b expansion of k uses only
 *     decimal digits 0–9, and
 *   • in base 16 the expansion contains at least one digit ≥ 10.
 * The sequence consists of all such integers, indexed starting at 1.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: test whether a sequence value uses only decimal digits in base 2..15
 * and has at least one non‑decimal digit in base 16. */
static bool accept(seq_t n)
{
    /* Bases 2 through 15: all digits must be < 10. */
    for (int base = 2; base <= 15; ++base)
    {
        seq_t t = n;
        while (t > 0)
        {
            seq_t r = t % (seq_t)base;
            if (r >= 10)
                return false;
            t /= (seq_t)base;
        }
    }

    /* Base 16: at least one digit must be ≥ 10. */
    seq_t t = n;
    bool has_nondec = false;
    while (t > 0)
    {
        seq_t r = t % (seq_t)16;
        if (r >= 10)
        {
            has_nondec = true;
            break;
        }
        t /= (seq_t)16;
    }
    return has_nondec;
}

/* The sequence cache. */
static seq_t *cache = NULL;        /* array of terms */
static idx_t cache_len = 0;        /* number of cached terms */
static idx_t cache_cap = 0;        /* allocated capacity */

/* Ensure the cache can hold at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;
    /* Grow cache capacity. */
    if (cache_cap == 0)
        cache_cap = 32;
    while (cache_cap < n)
        cache_cap *= 2;
    cache = mem_resize(seq_t, cache, cache_cap);
    /* Now extend the cache. */
    seq_t cand = cache_len > 0 ? cache[cache_len - 1] + 1 : 1;
    while (cache_len < n)
    {
        while (!accept(cand))
            cand++;
        cache[cache_len++] = cand;
        cand++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; undefined for 0, return 0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
