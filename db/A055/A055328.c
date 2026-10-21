/*
 * A055328  Number of rooted identity trees with n nodes and 3 leaves.
 * https://oeis.org/A055328
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-5) + 2*a(n-6) -
 *            3*a(n-7) + a(n-8) with a(6)=1, a(7)=5, a(8)=13, a(9)=28,
 *            a(10)=53, a(11)=91, a(12)=146, a(13)=223. For n<6 the value is
 *            undefined; this implementation returns 0
 * Range:     u8 n <= 13, u16 n <= 44, u32 n <= 676, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t const base_vals[14] = {
    0, 0, 0, 0, 0, 0,
    1, 5, 13, 28, 53, 91, 146, 223
};

static seq_t *cache = NULL;
static size_t cache_size = 0;       /* number of elements stored (max index + 1) */

/* ensure cache has space for indices [0..n] */
static void ensure_cache(size_t n)
{
    if (cache_size > n)
        return;                      /* already big enough */
    size_t new_size = n + 1;
    if (new_size < 64)
        new_size = 64;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* initialize new elements to 0 */
    for (size_t i = cache_size; i < new_size; ++i)
        cache[i] = 0;
    cache_size = new_size;
}

/* compute sequence up to index n */
static void compute_up_to(idx_t n)
{
    ensure_cache((size_t)n);
    size_t last = 5;          /* we already know indices 0..5 (undefined) */
    /* initialize known base terms */
    for (size_t i = 6; i <= 13 && i <= (size_t)n; ++i)
        cache[i] = base_vals[i];
    if (cache_size > 13)
        last = 13;
    else
        last = 5;              /* nothing computed yet beyond base */

    /* compute remaining terms */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        if (i <= 13) {
            cache[i] = base_vals[i];   /* redundant but safe */
            continue;
        }
        /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) - a(i-3)
                          + a(i-5) + 2*a(i-6) - 3*a(i-7) + a(i-8) */
        seq_t t1 = cache[i-1];
        seq_t t2 = cache[i-2];
        seq_t t3 = cache[i-3];
        seq_t t5 = cache[i-5];
        seq_t t6 = cache[i-6];
        seq_t t7 = cache[i-7];
        seq_t t8 = cache[i-8];

        seq_t val = 3 * t1;
        val -= 2 * t2;
        val -= t3;
        val += t5;
        val += 2 * t6;
        val -= 3 * t7;
        val += t8;
        cache[i] = val;
    }
}

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;                /* undefined for indices below offset */

    compute_up_to(n);
    return cache[(size_t)n];
}
