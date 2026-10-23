/*
 * A021299  Decimal expansion of 1/295.
 * https://oeis.org/A021299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/295) mod 10 (decimal digit of 1/295)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of already computed digits.  Each
 * call to a(n) expands the cache if necessary, computing new digits via
 * the standard long‑division method:
 *
 *   remainder = 1;
 *   digit_n   = (remainder * 10) / 295;
 *   remainder = (remainder * 10) % 295;
 *
 * This algorithm works for any seq_t that supports +, -, *, /, % on
 * integer values (including arbitrary‑precision types).
 */

#include <stdlib.h>   /* for malloc, free, exit */

static seq_t *cache = NULL;      /* cached digits */
static idx_t  cache_size = 0;    /* capacity of cache */
static idx_t  filled = 0;       /* number of digits already computed */
static seq_t  remainder = (seq_t)1;   /* remainder before next digit */

/* Ensure the cache contains at least indices 0..n. */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary. */
    if (n + 1 > cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp) { abort(); }   /* allocation failure */
        if (cache)
        {
            for (idx_t i = 0; i < cache_size; ++i)
                tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing digits. */
    for (idx_t i = filled; i <= n; ++i)
    {
        seq_t prod   = remainder * (seq_t)10;
        seq_t digit  = prod / (seq_t)295;
        cache[i] = digit;
        remainder = prod % (seq_t)295;
        ++filled;
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
