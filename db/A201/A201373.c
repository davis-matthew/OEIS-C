/*
 * A201373  Number of n X 6 0..1 arrays with rows and columns
 *          lexicographically nondecreasing read forwards, and nonincreasing
 *          read backwards.
 * https://oeis.org/A201373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 26, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 11*a(n-1) -55*a(n-2) +165*a(n-3) -330*a(n-4) +462*a(n-5) -462*a(n-6) +
   330*a(n-7) -165*a(n-8) +55*a(n-9) -11*a(n-10) + a(n-11)
   (for n > 11) with initial values
   a(1)=2, a(2)=7, a(3)=32, a(4)=157, a(5)=786,
   a(6)=3739, a(7)=15574, a(8)=55410, a(9)=170616,
   a(10)=465037, a(11)=1145954. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* 1‑based values stored at cache[n-1] */
static size_t cache_size = 0;        /* number of elements stored */
static size_t cache_cap   = 0;        /* allocated capacity of cache */

/* Initial values as 32‑bit integers to avoid overflow during declaration */
static const uint32_t base_vals[11] = {
    2,          /* a(1) */
    7,          /* a(2) */
    32,         /* a(3) */
    157,        /* a(4) */
    786,        /* a(5) */
    3739,       /* a(6) */
    15574,      /* a(7) */
    55410,      /* a(8) */
    170616,     /* a(9) */
    465037,     /* a(10) */
    1145954     /* a(11) */
};

/* Coefficients for the recurrence, signed as in the formula */
static const int coeff[11] = {
    11, -55, 165, -330, 462, -462, 330, -165, 55, -11, 1
};

/* Allocate or grow cache to hold at least 'needed' terms */
static void ensure_capacity_to(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise cache with the first 11 terms */
static void init_cache(void)
{
    if (cache)
        return;
    cache_cap = 16;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) /* in case of out‑of‑memory, leave empty */
        return;
    for (size_t i = 0; i < 11; ++i)
        cache[i] = (seq_t)base_vals[i];
    cache_size = 11;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined, but keep behaviour consistent */

    init_cache();

    /* Return if already computed */
    if ((idx_t)cache_size >= n)
        return cache[n - 1];

    /* Compute new terms up to n */
    for (idx_t idx = (idx_t)cache_size + 1; idx <= n; ++idx)
    {
        ensure_capacity_to((size_t)idx);
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (int d = 1; d <= 11; ++d)
        {
            seq_t val = cache[(size_t)(idx - d - 1)];   /* a(n-d) */
            int c = coeff[d - 1];
            unsigned int cu = c < 0 ? -(c) : c;         /* absolute value */
            seq_t prod = val * (seq_t)cu;
            if (c >= 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
        /* sum_pos >= sum_neg for all terms that fit; compute difference */
        cache[(size_t)idx - 1] = sum_pos - sum_neg;
    }
    cache_size = (size_t)n;
    return cache[n - 1];
}
