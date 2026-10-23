/*
 * A194224  Partial sums of A194223.
 * https://oeis.org/A194224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-12) - 2*a(n-13) + a(n-14);
 *            Initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=4, a(6)=6,
 *            a(7)=8, a(8)=11, a(9)=14, a(10)=18, a(11)=23, a(12)=28,
 *            a(13)=33, a(14)=38
 * Range:     u8 n <= 35, u16 n <= 561, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[15] = {
    0,  /* 0th, unused */
    0,  /* a(1) */
    0,  /* a(2) */
    1,  /* a(3) */
    2,  /* a(4) */
    4,  /* a(5) */
    6,  /* a(6) */
    8,  /* a(7) */
    11, /* a(8) */
    14, /* a(9) */
    18, /* a(10) */
    23, /* a(11) */
    28, /* a(12) */
    33, /* a(13) */
    38  /* a(14) */
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t last_comp = 14;

seq_t a(idx_t n)
{
    if (n <= 14) {
        return init_vals[(size_t)n];
    }

    /* Lazy initialization of cache */
    if (!cache) {
        cache_cap = 16; /* at least index 15 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, unlikely */
        for (int i = 1; i <= 14; ++i) {
            cache[i] = init_vals[i];
        }
        last_comp = 14;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = last_comp + 1; i <= (size_t)n; ++i) {
        cache[i] = ((seq_t)2 * cache[i-1]) - cache[i-2] +
                  cache[i-12] - ((seq_t)2 * cache[i-13]) + cache[i-14];
    }
    last_comp = (size_t)n;
    return cache[n];
}
