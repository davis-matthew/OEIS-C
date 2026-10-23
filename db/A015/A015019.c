/*
 * A015019  q-factorial numbers for q=-6.
 * https://oeis.org/A015019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = \prod_{k=1..n} ((-6)^k - 1)/(-6-1)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t   seq_cache_max = 0;
static seq_t   seq_power = 1;   /* (-6)^0 */

seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (seq_cache == NULL)
    {
        seq_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache_cap = 1;
        seq_cache[0]   = (seq_t)1; /* a(0) = 1 */
        seq_cache_max  = 0;
        seq_power      = (seq_t)1; /* (-6)^0 */
    }

    if (n > seq_cache_max)
    {
        size_t needed = (size_t)(n + 1);
        if (needed > seq_cache_cap)
        {
            seq_t *tmp = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
            if (!tmp) abort();
            seq_cache = tmp;
            seq_cache_cap = needed;
        }

        seq_t  val   = seq_cache[seq_cache_max]; /* a(seq_cache_max) */
        seq_t  power = seq_power;                /* (-6)^{seq_cache_max} */

        for (idx_t k = seq_cache_max + 1; k <= n; ++k)
        {
            power = power * (seq_t)(-6);            /* (-6)^k */
            seq_t factor = (seq_t)(1 - power) / (seq_t)7; /* ((-6)^k-1)/(-7) */
            val = val * factor;                     /* a(k) */
            seq_cache[k] = val;
        }

        seq_cache_max = n;
        seq_power = power; /* store (-6)^n for future extensions */
    }

    return seq_cache[n];
}
