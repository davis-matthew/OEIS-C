/*
 * A043814  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 8 mod 9.
 * https://oeis.org/A043814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043814: numbers whose number of runs in the base‑3 representation is
 * congruent to 8 (mod 9).  The function builds the sequence lazily, caching
 * results for independent calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t candidate   = 1;     /* next number to test, starts at 1 */

/* property test: runs in base‑3 representation ≡ 8 (mod 9) */
static bool cond(seq_t v)
{
    if (v == 0)
        return false;               /* 0 has no runs, but is not in the sequence */

    int digits[128];                /* enough for up to ~128‑bit numbers */
    int len = 0;
    seq_t t = v;

    /* convert to base‑3, least significant digit first */
    while (t > 0) {
        digits[len++] = (int)(t % 3);
        t /= 3;
    }

    /* count runs from most significant to least significant digit */
    int runs = 1;                   /* at least one run */
    int prev = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        int d = digits[i];
        if (d != prev) {
            ++runs;
            prev = d;
        }
    }

    return (runs % 9) == 8;
}

/* extend the cache to contain at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(1);                 /* allocation failure */
    }

    while (cache_len < (size_t)n) {
        while (!cond(candidate))
            ++candidate;              /* skip non‑qualifying numbers */

        if (cache_len == cache_cap) {  /* grow the array */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = candidate;
        ++candidate;                  /* start from the next number next time */
    }
}

/* required public function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of range */
    ensure_cache(n);
    return cache[n - 1];
}
