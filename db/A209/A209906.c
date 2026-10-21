/*
 * A209906  Half the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having two or four distinct clockwise edge differences.
 * https://oeis.org/A209906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 21*a(n-2) - 30*a(n-3) - 30*a(n-4) + 12*a(n-5)
 *            + 8*a(n-6) Initial values: a(1)=62, a(2)=562, a(3)=5008,
 *            a(4)=44770, a(5)=399930, a(6)=3573388
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The build supplies the types seq_t and idx_t */

static seq_t *cache = NULL;     /* array holding a(1) .. a(k) */
static size_t cache_cap = 0;    /* allocated size of cache array   */
static idx_t  computed = 0;     /* number of terms stored (max index+1) */

/* Ensure the cache has room for at least 'needed' terms (needed >= 1). */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 6;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first six terms. */
static void init_cache(void)
{
    grow_cache(6);
    cache[0] = (seq_t)62;
    cache[1] = (seq_t)562;
    cache[2] = (seq_t)5008;
    cache[3] = (seq_t)44770;
    cache[4] = (seq_t)399930;
    cache[5] = (seq_t)3573388;
    computed = 6;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) is outside the documented range */

    if (computed == 0)
        init_cache();

    if ((idx_t)computed >= n)
        return cache[n - 1];

    grow_cache((size_t)n);

    for (idx_t i = computed; i < n; ++i)
    {
        /* i corresponds to a(i+1), indices in array are i-1, i-2, ... */
        size_t idx = (size_t)i;
        seq_t pos = 7 * cache[idx - 1] + 21 * cache[idx - 2] +
                    12 * cache[idx - 5] + 8 * cache[idx - 6];
        seq_t sub = 30 * cache[idx - 3] + 30 * cache[idx - 4];
        cache[idx] = pos - sub;                /* unsigned subtraction is safe:
                                                  Pos >= Sub for all n >= 7   */
    }
    computed = n;
    return cache[n - 1];
}
