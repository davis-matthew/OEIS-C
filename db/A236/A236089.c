/*
 * A236089  Number of (n+1)X(1+1) 0..2 arrays colored with the difference of
 *          the maximum and the upper median in each 2X2 subblock.
 * https://oeis.org/A236089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-3) +220*a(n-4) +120*a(n-5) +736*a(n-6) +464*a(n-7)
 *            -176*a(n-8) +224*a(n-9) -256*a(n-10) for n>11 initial values
 *            (n=1..11): a(1)=81, a(2)=344, a(3)=1512, a(4)=6444, a(5)=28060,
 *            a(6)=119928, a(7)=519616, a(8)=2226672, a(9)=9647072,
 *            a(10)=41385504, a(11)=178859072
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;    /* array holding computed a(n), 1‑based */
static idx_t cache_capacity = 0;   /* size of allocated array (indices 0..capacity-1) */
static idx_t cache_max = 0;        /* largest n for which a(n) is computed */

static void ensure_cache(idx_t n)
{
    if (n < cache_capacity)
        return;
    /* grow: at least double, but at least enough for n+1 indices */
    idx_t new_cap = cache_capacity ? cache_capacity << 1 : 16;
    if (new_cap <= n)
        new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        return;                      /* allocation failed; caller will handle 0 */
    /* initialise new slots to zero */
    for (idx_t i = cache_capacity; i < new_cap; ++i)
        tmp[i] = 0;
    seq_cache = tmp;
    cache_capacity = new_cap;
}

static const seq_t init_vals[12] =
{
    0,  /* a(0) unused */
    81, 344, 1512, 6444, 28060, 119928,
    519616, 2226672, 9647072, 41385504, 178859072
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;    /* undefined for n<1 but guard */

    if (!seq_cache)
        seq_cache = (seq_t *)calloc(16, sizeof(seq_t)); /* initial capacity */
    if (!seq_cache)
        return 0;    /* allocation failed */

    ensuring_cache: ;
    ensure_cache(n);
    if (cache_max >= n)
        return seq_cache[n];

    /* compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        if (i <= 11)
        {
            seq_cache[i] = init_vals[(int)i];
        }
        else
        {
            seq_t v = 0;
            v += 12   * seq_cache[i-3];
            v += 220  * seq_cache[i-4];
            v += 120  * seq_cache[i-5];
            v += 736  * seq_cache[i-6];
            v += 464  * seq_cache[i-7];
            v -= 176  * seq_cache[i-8];
            v += 224  * seq_cache[i-9];
            v -= 256  * seq_cache[i-10];
            seq_cache[i] = v;
        }
    }
    cache_max = n;
    return seq_cache[n];
}
