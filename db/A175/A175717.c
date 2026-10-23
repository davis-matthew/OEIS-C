/*
 * A175717  First differences of A175628.
 * https://oeis.org/A175717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 375, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0)=0, a(1)=3, a(2)=2, a(3)=3, a(4)=-5, a(5)=12, a(6)=6, a(7)=3,
   a(8)=-22, a(9)=33, a(10)=10, a(11)=3, a(12)=-33, a(13)=48, a(14)=14, a(15)=3,
   a(16)=-74, a(17)=93, a(18)=18, a(19)=3, a(20)=-85, a(21)=108, a(22)=22, a(23)=3;
   for n ≥ 24: a(n) = 3*a(n-8) – 3*a(n-16) + a(n-24). */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;     /* number of valid entries in cache */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < n + 1)
        new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Base values for indices 0 … 23. */
static inline seq_t
base_value(idx_t i)
{
    switch ((unsigned)i)
    {
        case 0:  return (seq_t)0;
        case 1:  return (seq_t)3;
        case 2:  return (seq_t)2;
        case 3:  return (seq_t)3;
        case 4:  return (seq_t)(-5);
        case 5:  return (seq_t)12;
        case 6:  return (seq_t)6;
        case 7:  return (seq_t)3;
        case 8:  return (seq_t)(-22);
        case 9:  return (seq_t)33;
        case 10: return (seq_t)10;
        case 11: return (seq_t)3;
        case 12: return (seq_t)(-33);
        case 13: return (seq_t)48;
        case 14: return (seq_t)14;
        case 15: return (seq_t)3;
        case 16: return (seq_t)(-74);
        case 17: return (seq_t)93;
        case 18: return (seq_t)18;
        case 19: return (seq_t)3;
        case 20: return (seq_t)(-85);
        case 21: return (seq_t)108;
        case 22: return (seq_t)22;
        case 23: return (seq_t)3;
    }
    return (seq_t)0;  /* unreachable */
}

seq_t
a(idx_t n)
{
    /* Make sure the cache can handle index n. */
    ensure_cache(n);

    /* Compute any missing values up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 24)
            cache[i] = base_value(i);
        else
            cache[i] = 3 * cache[i - 8] - 3 * cache[i - 16] + cache[i - 24];
    }
    cache_len = n + 1;
    return cache[n];
}
