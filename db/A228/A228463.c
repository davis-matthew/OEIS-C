/*
 * A228463  Number of arrays of maxima of three adjacent elements of some
 *          length 8 0..n array.
 * https://oeis.org/A228463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 65, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4)
        + 21*a(n-5) - 7*a(n-6) + a(n-7)   (n>7)
   Initial values: a(1)=27, a(2)=183, a(3)=736, a(4)=2227, a(5)=5615,
   a(6)=12453, a(7)=25096 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

static const seq_t base_vals[7] = {
    (seq_t)27,     /* a(1) */
    (seq_t)183,    /* a(2) */
    (seq_t)736,    /* a(3) */
    (seq_t)2227,   /* a(4) */
    (seq_t)5615,   /* a(5) */
    (seq_t)12453,  /* a(6) */
    (seq_t)25096   /* a(7) */
};

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    size_t idx = (size_t)n;

    /* Ensure capacity for the desired index. */
    ensure_cap(idx);

    /* If the value is already cached, return it. */
    if (idx <= cache_size)
        return cache[idx - 1];

    /* Compute missing values up to idx. */
    for (size_t i = cache_size + 1; i <= idx; ++i)
    {
        if (i <= 7)
        {
            cache[i - 1] = base_vals[i - 1];
        }
        else
        {
            /* Recurrence: a(i) = 7*a(i-1) - 21*a(i-2) + 35*a(i-3)
                               - 35*a(i-4) + 21*a(i-5) - 7*a(i-6) + a(i-7) */
            seq_t v = cache[i - 2] * (seq_t)7;
            v -= cache[i - 3] * (seq_t)21;
            v += cache[i - 4] * (seq_t)35;
            v -= cache[i - 5] * (seq_t)35;
            v += cache[i - 6] * (seq_t)21;
            v -= cache[i - 7] * (seq_t)7;
            v += cache[i - 8];
            cache[i - 1] = v;
        }
    }
    cache_size = idx;
    return cache[idx - 1];
}
