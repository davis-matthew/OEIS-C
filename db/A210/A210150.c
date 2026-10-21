/*
 * A210150  1/4 the number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock
 *          having one, three or four distinct clockwise edge differences.
 * https://oeis.org/A210150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 49*a(n-1) + 200*a(n-2) - 2256*a(n-3) - 3750*a(n-4) +
 *            29143*a(n-5) - 5802*a(n-6) - 84658*a(n-7) + 57585*a(n-8) +
 *            56482*a(n-9) - 40584*a(n-10) - 14146*a(n-11) + 6952*a(n-12) +
 *            1280*a(n-13) (empirical recurrence). * Initial values: a(1)=820,
 *            a(2)=42689, a(3)=2219034, a(4)=115368738, a(5)=5997...
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const char *const init_str[13] = {
    "820",
    "42689",
    "2219034",
    "115368738",
    "5997955680",
    "311831135649",
    "16211961522199",
    "842852674077118",
    "43819535708838103",
    "2278158176293618573",
    "118440430549650177612",
    "6157665317047215194813",
    "320134281687114509018719"
};

static seq_t *cache = NULL;   /* 0‑based cache of computed terms */
static size_t cache_len = 0;  /* number of terms stored (a(1) at index 0) */
static size_t cache_cap = 0;  /* allocated capacity */

static void init_cache(void)
{
    cache_cap = 13;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    for (size_t i = 0; i < 13; ++i)
        cache[i] = seq_from_str(init_str[i]);
    cache_len = 13;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)            /* undefined for n < 1, return 0 */
        return (seq_t)0;

    if (cache == NULL)           /* first call */
        init_cache();

    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Extend cache capacity if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 32;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i is zero‑based index for current term a(i+1) */
        size_t idx = i + 1;          /* term number (a(idx)) */
        seq_t pos = 0, neg = 0;
        seq_t tmp;

        /* a(idx-1) */
        tmp = cache[idx-2];
        pos += (seq_t)49 * tmp;
        /* a(idx-2) */
        tmp = cache[idx-3];
        pos += (seq_t)200 * tmp;
        /* a(idx-3) */
        tmp = cache[idx-4];
        neg += (seq_t)2256 * tmp;
        /* a(idx-4) */
        tmp = cache[idx-5];
        neg += (seq_t)3750 * tmp;
        /* a(idx-5) */
        tmp = cache[idx-6];
        pos += (seq_t)29143 * tmp;
        /* a(idx-6) */
        tmp = cache[idx-7];
        neg += (seq_t)5802 * tmp;
        /* a(idx-7) */
        tmp = cache[idx-8];
        neg += (seq_t)84658 * tmp;
        /* a(idx-8) */
        tmp = cache[idx-9];
        pos += (seq_t)57585 * tmp;
        /* a(idx-9) */
        tmp = cache[idx-10];
        pos += (seq_t)56482 * tmp;
        /* a(idx-10) */
        tmp = cache[idx-11];
        neg += (seq_t)40584 * tmp;
        /* a(idx-11) */
        tmp = cache[idx-12];
        neg += (seq_t)14146 * tmp;
        /* a(idx-12) */
        tmp = cache[idx-13];
        pos += (seq_t)6952 * tmp;
        /* a(idx-13) */
        tmp = cache[idx-14];
        pos += (seq_t)1280 * tmp;

        cache[i] = pos - neg;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
