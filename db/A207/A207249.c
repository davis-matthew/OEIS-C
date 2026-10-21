/*
 * A207249  Number of n X 3 0..1 arrays avoiding 0 0 1 and 1 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 7*a(n-4) + 8*a(n-5) - a(n-8)
 *            (offset 1) initial values: a(1)=6, a(2)=36, a(3)=102, a(4)=370,
 *            a(5)=1232, a(6)=4238, a(7)=14406, a(8)=49164
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of values with indices 0.. */
static idx_t  cache_size = 0;    /* highest n computed (>=0) */
static size_t cache_cap  = 0;    /* allocated size (>= cache_size+1) */

static void ensure_capacity(idx_t need)
{
    /* need is the greatest index that will be accessed (n) */
    if ((size_t)need + 1 <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap * 2 : 2;
    while (newcap <= (size_t)need + 1)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = newcap;
}

/* Compute a(n) for n >= 1; for n < 1 return 0 (undefined but safe) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Initialize cache on first call */
    if (!cache) {
        /* We need indices 0..8 available; store 0 at index 0 for convenience */
        cache_cap = 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        static const seq_t init_vals[9] = {
            0,          /* unused placeholder for index 0 */
            6, 36, 102, 370, 1232, 4238, 14406, 49164
        };
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_size = 8;      /* highest pre‑computed index */
    }

    if (n <= cache_size)
        return cache[n];

    /* Allocate more space if necessary */
    ensure_capacity(n);

    /* Compute missing values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = 0;
        /* Use the recurrence, all needed previous indices are defined */
        val += (seq_t)2 * cache[i - 1];
        val += (seq_t)4 * cache[i - 2];
        val += (seq_t)7 * cache[i - 4];
        val += (seq_t)8 * cache[i - 5];
        val -= cache[i - 8];
        cache[i] = val;
    }
    cache_size = n;
    return cache[n];
}
