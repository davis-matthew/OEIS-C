/*
 * A231051  Number of 3Xn 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4 and at least one element with value
 *          (x(i,j)-1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 2, a(3) = 50, a(4) = 128, a(5) = 644, a(6) =
 *            5100, a(7) = 33696, a(8) = 211032, a(9) = 1354848, a(10) =
 *            8755256, a(11) = 56382600, a(n) = 6*a(n-1) + 18*a(n-3) +
 *            17*a(n-4) - 98*a(n-5) - 32*a(n-6) + 6*a(n-7) - 16*a(n-8) +
 *            36*a(n-9) - 8*a(n-10) for n > 11 (OEIS A231051) The implemen...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;     /* 1‑based cache, cache[0] unused */
static idx_t cache_size = 0;    /* highest index currently cached */

/* Ensure that cache has entries up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already cached */

    idx_t old_size = cache_size;
    idx_t new_size = n;

    /* Reallocate the cache to hold indices 0 … new_size */
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));

    /* Initialise the first 11 terms (the recurrence is only valid for n>11) */
    static const seq_t init[12] = {
        0,          /* cache[0] unused */
        0,          /* a(1) */
        2,          /* a(2) */
        50,         /* a(3) */
        128,        /* a(4) */
        644,        /* a(5) */
        5100,       /* a(6) */
        33696,      /* a(7) */
        211032,     /* a(8) */
        1354848,    /* a(9) */
        8755256,    /* a(10) */
        56382600    /* a(11) */
    };

    /* Fill any missing terms up to new_size */
    for (idx_t i = 1; i <= 11 && i <= new_size; ++i)
        cache[i] = init[i];

    /* Compute terms for indices > 11 using the empirical recurrence */
    for (idx_t i = old_size + 1; i <= new_size; ++i)
    {
        if (i <= 11) continue;   /* already initialised */

        seq_t val = (seq_t)6 * cache[i-1];
        val += (seq_t)18 * cache[i-3];
        val += (seq_t)17 * cache[i-4];
        val -= (seq_t)98 * cache[i-5];
        val -= (seq_t)32 * cache[i-6];
        val += (seq_t)6  * cache[i-7];
        val -= (seq_t)16 * cache[i-8];
        val += (seq_t)36 * cache[i-9];
        val -= (seq_t)8  * cache[i-10];
        cache[i] = val;
    }

    cache_size = new_size;
}

/* Public function: return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
