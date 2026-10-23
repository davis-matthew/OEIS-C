/*
 * A160577  Positive numbers y such that y^2 is of the form x^2+(x+409)^2 with
 *          integer x.
 * https://oeis.org/A160577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):  a(1)=305, a(2)=409, a(3)=641, a(4)=1189,
 * a(5)=2045, a(6)=3541; and for n > 6    a(n) = 6*a(n-3) - a(n-6) */
#include <stddef.h>

/* cache large enough for the typical test range (n up to ~10000)          */
static seq_t cache[20001];          /* indices 0 … 20000 correspond to a(1)…a(20001) */
static idx_t cache_size = 0;        /* number of terms currently stored (max 6 after init) */

/* initialise the first six terms of the sequence */
static void init_cache(void)
{
    if (cache_size != 0)
        return;                     /* already initialised */

    cache[0] = (seq_t)305;
    cache[1] = (seq_t)409;
    cache[2] = (seq_t)641;
    cache[3] = (seq_t)1189;
    cache[4] = (seq_t)2045;
    cache[5] = (seq_t)3541;
    cache_size = 6;
}

/* compute terms up to and including n (n is guaranteed > cache_size) */
static void compute_up_to(idx_t n)
{
    /* make sure the first six terms are present */
    if (cache_size == 0)
        init_cache();

    /* cache_size < n  and  cache_size >= 6.  We start at i=cache_size+1  */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* a(i) = 6*a(i-3) - a(i-6) */
        cache[i - 1] = 6 * cache[i - 4] - cache[i - 7];
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1, return 0 to be safe */
        return (seq_t)0;

    init_cache();

    if (n <= cache_size)
        return cache[n - 1];

    compute_up_to(n);
    return cache[n - 1];
}
