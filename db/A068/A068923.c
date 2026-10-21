/*
 * A068923  Number of ways to tile a 4 X n room with 1x2 Tatami mats. At most
 *          3 Tatami mats may meet at a point.
 * https://oeis.org/A068923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-3) + a(n-5) for n ≥ 9 Initial values: a(1)=1, a(2)=4,
 *            a(3)=4, a(4)=2, a(5)=3, a(6)=3, a(7)=3, a(8)=5
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] is unused */
static idx_t cache_size = 0;

/* make sure the cache can hold values up to n (inclusive)          */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    idx_t new_size = n + 1; /* indices 0 .. n */
    /* realloc returns void* so cast explicitly for C++               */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort                            */
        abort();
    }
    /* initialize the newly added slots to zero */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_size = new_size;
}

/* set the known initial terms (only once)                           */
static void init_base(void)
{
    /* ensure cache can hold at least indices 1..8 */
    ensure_cache(8);

    cache[1] = 1;
    cache[2] = 4;
    cache[3] = 4;
    cache[4] = 2;
    cache[5] = 3;
    cache[6] = 3;
    cache[7] = 3;
    cache[8] = 5;
}

/* compute a(n) using the recurrence a(n)=a(n-3)+a(n-5) for n>=9   */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n<1 */

    ensure_cache(n);
    /* initialise base values on first call */
    if (cache[1] == 0)
        init_base();

    /* compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 8)
            continue;                  /* already known */
        if (cache[i] != 0)
            continue;                  /* already computed */
        /* recurrence uses i-3 and i-5 which are < i, so they are ready */
        cache[i] = cache[i - 3] + cache[i - 5];
    }
    return cache[n];
}
