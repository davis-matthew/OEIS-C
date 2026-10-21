/*
 * A200273  Number of 0..n arrays x(0..5) of 6 elements with zero 3rd
 *          differences.
 * https://oeis.org/A200273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3)+a(n-5)+a(n-6)-a(n-8)-a(n-9)+a(n-10)
 *            -a(n-11)-a(n-13)+a(n-14)-a(n-15)-a(n-16)+a(n-18)
 *            +a(n-19)+a(n-21)-a(n-24) Initial values:
 *            a(1)=2,a(2)=3,a(3)=6,a(4)=9,a(5)=14,a(6)=25,
 *            a(7)=36,a(8)=47,a(9)=64,a(10)=87,a(11)=110,
 *            a(12)=143,a(13)=176,a(14)=209,a(15)=258 (and so on)
 * Range:     u8 n <= 14, u16 n <= 101, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system.  No need for typedefs here. */

/* Cached sequence values, 0‑based index (a(n) is stored at cache[n-1]). */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms currently stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* 24 known terms of the sequence. */
static const seq_t init_vals[24] = {
    /* a(1)..a(24) */
    2ULL, 3ULL, 6ULL, 9ULL, 14ULL, 25ULL, 36ULL, 47ULL, 64ULL, 87ULL,
    110ULL, 143ULL, 176ULL, 209ULL, 258ULL, 311ULL, 364ULL, 431ULL,
    498ULL, 575ULL, 666ULL, 761ULL, 856ULL, 969ULL
};

/* Ensure cache is large enough to contain 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – terminate program. */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main entry point: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n==0 – placeholder */

    /* Grow cache if necessary. */
    if ((size_t)n > cache_len) {
        ensure_capacity((size_t)n);

        /* Fill missing entries. */
        for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
            seq_t val;
            if (i <= 24) {
                val = init_vals[i - 1];
            } else {
                /* Recurrence calculation.  All indices are >=1 here. */
                val = cache[i - 3 - 1];
                val += cache[i - 5 - 1];
                val += cache[i - 6 - 1];
                val += cache[i - 10 - 1];
                val += cache[i - 14 - 1];
                val += cache[i - 18 - 1];
                val += cache[i - 19 - 1];
                val += cache[i - 21 - 1];
                val -= cache[i - 8 - 1];
                val -= cache[i - 9 - 1];
                val -= cache[i - 11 - 1];
                val -= cache[i - 13 - 1];
                val -= cache[i - 15 - 1];
                val -= cache[i - 16 - 1];
                val -= cache[i - 24 - 1];
            }
            cache[i - 1] = val;
        }
        cache_len = (size_t)n;  /* now we have terms up to n */
    }
    return cache[n - 1];
}
