/*
 * A016093  Expansion of 1/((1-8*x)*(1-10*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A016093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=41, a(2)=1055, a(3)=21805 a(n) = 41*a(n-1) -
 *            626*a(n-2) + 4216*a(n-3) - 10560*a(n-4), n>=4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* array of computed terms   */
static size_t cache_size = 0;     /* number of terms stored    */
static size_t cache_capacity = 0; /* allocated array size      */

/* ensure that the cache holds at least 'need+1' elements              */
static void ensure_cache_capacity(size_t need)
{
    if (cache_capacity > need)
        return;
    size_t newcap = cache_capacity ? cache_capacity * 2 : 64;
    while (newcap <= need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache = tmp;
    cache_capacity = newcap;
}

/* initialize cache with the first four terms exactly once             */
static void init_cache(void)
{
    if (cache_size == 0)
    {
        /* allocate space for at least 4 elements */
        size_t need = 3;
        size_t newcap = 64;
        while (newcap <= need)
            newcap <<= 1;
        cache = (seq_t *)malloc(newcap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)41;
        cache[2] = (seq_t)1055;
        cache[3] = (seq_t)21805;
        cache_size     = 4;
        cache_capacity = newcap;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    /* fast return if already computed */
    if (n < cache_size)
        return cache[n];

    /* compute terms up to n using the linear recurrence */
    while (cache_size <= n)
    {
        /* make sure there is room for the next element */
        ensure_cache_capacity(cache_size);

        seq_t t1 = 41   * cache[cache_size - 1];
        seq_t t2 = 626  * cache[cache_size - 2];
        seq_t t3 = 4216 * cache[cache_size - 3];
        seq_t t4 = 10560* cache[cache_size - 4];

        seq_t val = t1 + t3;   /* 41*a(n‑1) + 4216*a(n‑3) */
        val -= t2;            /* subtract 626*a(n‑2)     */
        val -= t4;            /* subtract 10560*a(n‑4)   */

        cache[cache_size] = val;
        cache_size++;
    }

    return cache[n];
}
