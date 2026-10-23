/*
 * A199838  Number of -n..n arrays x(0..8) of 9 elements with zero sum and no
 *          two neighbors summing to zero.
 * https://oeis.org/A199838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            9, with a(1)=66, a(2)=23206, a(3)=645780, a(4)=6715618,
 *            a(5)=41008804, a(6)=179213048, a(7)=622300326, a(8)=1827026482,
 *            a(9)=4719970500
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    66ULL,        /* a(1) */
    23206ULL,    /* a(2) */
    645780ULL,   /* a(3) */
    6715618ULL,  /* a(4) */
    41008804ULL, /* a(5) */
    179213048ULL,/* a(6) */
    622300326ULL,/* a(7) */
    1827026482ULL,/* a(8) */
    4719970500ULL /* a(9) */
};

static seq_t *cache = NULL;      /* 0‑based storage for a(1) at cache[0] */
static idx_t cache_size = 0;     /* number of allocated slots */
static idx_t max_computed = 0;   /* largest n for which a(n) is in cache */

/* multiply by a small unsigned constant – safe for all seq_t types */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    return x * m;
}

/* ensure cache can hold up to `upto` elements (1‑based index) */
static void ensure_cache(idx_t upto)
{
    if (upto <= cache_size)
        return;

    /* grow capacity at least twice, but not less than 9 */
    idx_t new_cap = (cache_size > 0) ? cache_size * 2 : 9;
    if (new_cap < upto)
        new_cap = upto;

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) abort();           /* unrecoverable allocation failure */

    /* if this is the first allocation, copy the first 9 terms */
    if (cache_size == 0) {
        for (idx_t i = 0; i < 9 && i < new_cap; ++i)
            tmp[i] = init[i];
    }

    cache = tmp;
    cache_size = new_cap;

    /* keep track of how many terms are already known */
    if (max_computed < 9 && cache_size >= 9)
        max_computed = 9;
}

/* compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(0) not defined */
        return 0;

    /* trivial case: use the hard‑coded initial values */
    if (n <= 9)
        return init[n - 1];

    /* make sure the cache can accommodate up to n */
    ensure_cache(n);

    /* fill values from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence computation (all arguments are 1‑based) */
        seq_t value = 0;
        value += mul_small(cache[i - 2], 9);   /* a(i-1) */
        value -= mul_small(cache[i - 3], 36);  /* a(i-2) */
        value += mul_small(cache[i - 4], 84);   /* a(i-3) */
        value -= mul_small(cache[i - 5], 126); /* a(i-4) */
        value += mul_small(cache[i - 6], 126); /* a(i-5) */
        value -= mul_small(cache[i - 7], 84);  /* a(i-6) */
        value += mul_small(cache[i - 8], 36);  /* a(i-7) */
        value -= mul_small(cache[i - 9], 9);   /* a(i-8) */
        value += cache[i - 10];               /* a(i-9) */

        cache[i - 1] = value;
    }

    max_computed = n;
    return cache[n - 1];
}
