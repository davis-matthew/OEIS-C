/*
 * A185462  Number of (n+1)X5 binary arrays with no 2X2 subblock determinant
 *          equal to any horizontal or vertical neighbor 2X2 subblock
 *          determinant.
 * https://oeis.org/A185462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-2) + 27*a(n-4) + 75*a(n-6) + 77*a(n-8) +
 *            32*a(n-10) + a(n-12) - a(n-14) for n > 20 with initial values:
 *            a(1)=218, a(2)=672, a(3)=2766, a(4)=13977, a(5)=49670,
 *            a(6)=231636, a(7)=798330, a(8)=3709535, a(9)=12773958,
 *            a(10)=59358300, a(11)=204384986, a(12)=949713803,
 *            a(13)=3270113366, a...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;        /* 0‑unused slot, data stored at indices 1.. */
static idx_t  cache_cap = 0;       /* allocated capacity (size of array)   */
static idx_t  cache_last = 0;      /* highest n whose value is already stored */

/* ensure capacity for indices up to n (inclusive) */
static void ensure_cap(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = n + 1;                 /* indices 0..n inclusive */
    seq_t *new_cached = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cached)
        abort();

    /* initialise new entries to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cached[i] = (seq_t)0;

    cache = new_cached;
    cache_cap = new_cap;
}

/* initialise base values once */
static void init_base(void)
{
    static bool once = false;
    if (once)
        return;

    ensure_cap(20);

    cache[1]  = (seq_t)218;
    cache[2]  = (seq_t)672;
    cache[3]  = (seq_t)2766;
    cache[4]  = (seq_t)13977;
    cache[5]  = (seq_t)49670;
    cache[6]  = (seq_t)231636;
    cache[7]  = (seq_t)798330;
    cache[8]  = (seq_t)3709535;
    cache[9]  = (seq_t)12773958;
    cache[10] = (seq_t)59358300;
    cache[11] = (seq_t)204384986;
    cache[12] = (seq_t)949713803;
    cache[13] = (seq_t)3270113366;
    cache[14] = (seq_t)15195208008;
    cache[15] = (seq_t)52321104606;
    cache[16] = (seq_t)243120127107;
    cache[17] = (seq_t)837126587718;
    cache[18] = (seq_t)3889870442680;
    cache[19] = (seq_t)13393847713706;
    cache[20] = (seq_t)62237101136015;

    cache_last = 20;
    once = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out‑of‑range guard */

    init_base();

    if (n <= cache_last)
        return cache[n];

    /* extend capacity to index n */
    ensure_cap(n);

    /* compute missing values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        cache[i] = 14 * cache[i-2] + 27 * cache[i-4] +
                   75 * cache[i-6] + 77 * cache[i-8] +
                   32 * cache[i-10] + cache[i-12] -
                   cache[i-14];
    }

    cache_last = n;
    return cache[n];
}
