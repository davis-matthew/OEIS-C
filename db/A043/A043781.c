/*
 * A043781  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 5.
 * https://oeis.org/A043781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑3 digit run count ≡ 1 (mod 5)
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* count_runs returns the number of runs in the base‑3 representation of x */
static int count_runs(seq_t x)
{
    const seq_t three = (seq_t)3;
    int runs = 0;
    int prev = -1;
    while (x > (seq_t)0) {
        seq_t d = x % three;            /* 0,1,2 */
        int digit = (int)d;
        if (runs == 0 || digit != prev) {
            runs++;
            prev = digit;
        }
        x /= three;
    }
    return runs;
}

/* dynamic cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of entries in cache */
static size_t cache_cap = 0;   /* allocated capacity */

/* ensure cache has at least needed capacity */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < needed) new_cap *= 2;
        seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newptr) abort();
        cache = newptr;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1, undefined for n==0 */

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* start from last computed number + 1 (or 1 if cache empty) */
        idx_t val = (cache_len == 0) ? 1 : (idx_t)(cache[cache_len - 1] + (seq_t)1);
        /* find next qualifying number */
        while (1) {
            if (count_runs((seq_t)val) % 5 == 1) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)val;
                break;
            }
            val++;
        }
    }

    return cache[n - 1];
}
