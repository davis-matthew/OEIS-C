/*
 * A183928  Number of nondecreasing arrangements of n numbers in -2..2 with
 *          sum zero and sum of squares less than 2n.
 * https://oeis.org/A183928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            a(n-1)+2*a(n-3)-a(n-4)-a(n-5)-a(n-6)-a(n-7)+2*a(n-8)+a(n-10)-a(n-11)
 *            (for n ≥ 12) with initial values a(1)=1, a(2)=2, a(3)=2, a(4)=5,
 *            a(5)=8, a(6)=10, a(7)=15, a(8)=21, a(9)=26, a(10)=35, a(11)=44.
 *            Uses a linear‑time recurrence and memoisation
 * Range:     u8 n <= 21, u16 n <= 150, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for already computed values.
   cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* actual allocated size (indices 0 .. cache_cap-1) */
static idx_t cached_up_to = 0;   /* largest n for which cache[n] is valid */

/* Ensure the cache has room for indices up to n. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* memory exhausted */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Helper array of initial values a(1)..a(11).  Index 0 is unused. */
static const seq_t init_vals[12] = {
    0,  /* unused */
    1,  /* a(1) */
    2,  /* a(2) */
    2,  /* a(3) */
    5,  /* a(4) */
    8,  /* a(5) */
    10, /* a(6) */
    15, /* a(7) */
    21, /* a(8) */
    26, /* a(9) */
    35, /* a(10) */
    44, /* a(11) */
    /* a(12) is not needed for the recurrence */
};

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;     /* undefined below the offset */

    ensure_capacity(n);

    /* Compute any values not yet cached */
    for (idx_t i = cached_up_to + 1; i <= n; ++i)
    {
        if (i <= 11)
        {
            cache[i] = init_vals[i];
        }
        else
        {
            /* Compute the recurrence using only positive terms first
               to avoid unsigned underflow. */
            seq_t pos = cache[i-1];
            pos += cache[i-3];
            pos += cache[i-3];      /* +2*a(i-3) */
            pos += cache[i-8];
            pos += cache[i-8];      /* +2*a(i-8) */
            pos += cache[i-10];     /* +a(i-10) */

            seq_t neg = cache[i-4];
            neg += cache[i-5];
            neg += cache[i-6];
            neg += cache[i-7];
            neg += cache[i-11];     /* +a(i-11) */

            cache[i] = pos - neg;
        }
    }

    cached_up_to = n;
    return cache[n];
}
