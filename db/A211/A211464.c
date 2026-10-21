/*
 * A211464  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having one, three or four distinct values for every i<=n and j<=n.
 * https://oeis.org/A211464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) -16*a(n-2) -118*a(n-3) +321*a(n-4) +560*a(n-5)
 *            -1724*a(n-6) -1636*a(n-7) +3492*a(n-8) +2984*a(n-9)
 *            -1216*a(n-10) -608*a(n-11) +192*a(n-12) Initial values (offset
 *            1): a(1)=21, a(2)=89, a(3)=353, a(4)=1373, a(5)=5225,
 *            a(6)=19645, a(7)=73105, a(8)=270125, a(9)=992545,
 *            a(10)=3631741,...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The following function implements this recurrence and memoises
    intermediate results so that repeated calls are fast. */

#include <stdlib.h>
#include <stdint.h>

/*=== memoisation state ====================================================*/
static seq_t *cache = NULL;      /* array of computed values, 1‑based */
static size_t cache_size = 0;    /* allocated size including index 0 */
static idx_t   computed_max = 0; /* highest index computed so far */

/* === Ensure the cache can hold at least n+1 elements (index 0 unused) ====*/
static void grow_cache(idx_t n)
{
    if (cache_size <= (size_t)n)
    {
        size_t new_size = cache_size ? cache_size * 2 : 16;
        while (new_size <= (size_t)n)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }
}

/* === Initialise the base terms (indices 1..12) ============================*/
static void init_base(void)
{
    if (computed_max >= 12)
        return;
    grow_cache(12);
    cache[0] = (seq_t)0;            /* unused */
    cache[1]  = (seq_t)21;
    cache[2]  = (seq_t)89;
    cache[3]  = (seq_t)353;
    cache[4]  = (seq_t)1373;
    cache[5]  = (seq_t)5225;
    cache[6]  = (seq_t)19645;
    cache[7]  = (seq_t)73105;
    cache[8]  = (seq_t)270125;
    cache[9]  = (seq_t)992545;
    cache[10] = (seq_t)3631741;
    cache[11] = (seq_t)13245457;
    cache[12] = (seq_t)48187085;
    computed_max = 12;
}

/* === The sequence implementation ========================================*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, but harmless */
    grow_cache(n);
    init_base();

    if (n <= 12)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        /* Positive contributions */
        pos += cache[i-1] * (seq_t)10;
        pos += cache[i-4] * (seq_t)321;
        pos += cache[i-5] * (seq_t)560;
        pos += cache[i-8] * (seq_t)3492;
        pos += cache[i-9] * (seq_t)2984;
        pos += cache[i-12] * (seq_t)192;

        /* Negative contributions */
        neg += cache[i-2] * (seq_t)16;
        neg += cache[i-3] * (seq_t)118;
        neg += cache[i-6] * (seq_t)1724;
        neg += cache[i-7] * (seq_t)1636;
        neg += cache[i-10] * (seq_t)1216;
        neg += cache[i-11] * (seq_t)608;

        cache[i] = pos - neg;   /* pos >= neg for all n >= 13 */
    }
    computed_max = n;
    return cache[n];
}
