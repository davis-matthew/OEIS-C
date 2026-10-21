/*
 * A224741  Number of (n+1)X5 0..1 matrices with each 2X2 permanent equal.
 * https://oeis.org/A224741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A224741:
   a(n) = 13*a(n-1)+a(n-2)-436*a(n-3)+747*a(n-4)+3955*a(n-5)
          -7874*a(n-6)-12461*a(n-7)+25451*a(n-8)+13120*a(n-9)
          -30315*a(n-10)-175*a(n-11)+9975*a(n-12)-2000*a(n-13)
   Initial terms (offset 1):
   a(1)=218, a(2)=2326, a(3)=18257, a(4)=180546, a(5)=1644874,
   a(6)=15845297, a(7)=149592266, a(8)=1430571502, a(9)=13614536577,
   a(10)=129950804946, a(11)=1238946474290, a(12)=11820093210897,
   a(13)=112737919290762   */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity   */
static size_t cache_size = 0;  /* number of terms stored */

static const seq_t init_vals[13] = {
    (seq_t)218,      (seq_t)2326,      (seq_t)18257,
    (seq_t)180546,   (seq_t)1644874,   (seq_t)15845297,
    (seq_t)149592266,(seq_t)1430571502,(seq_t)13614536577,
    (seq_t)129950804946,(seq_t)1238946474290,
    (seq_t)11820093210897,(seq_t)112737919290762
};

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* below offset, unused */

    /* Initialise cache once with the first 13 terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        cache_size = 13;
    }

    if ((size_t)n <= cache_size)
        return cache[n - 1];          /* n is 1‑based, cache is 0‑based */

    ensure_capacity((size_t)n);

    /* Compute terms from cache_size up to n-1 (zero‑based indices) */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* i corresponds to a[i+1] */
        seq_t pos = mul_small(cache[i - 1], 13);      /* +13*a(n-1) */
        pos += cache[i - 2];                          /* +1*a(n-2) */
        pos += mul_small(cache[i - 4], 747);           /* +747*a(n-4) */
        pos += mul_small(cache[i - 5], 3955);          /* +3955*a(n-5) */
        pos += mul_small(cache[i - 8], 25451);        /* +25451*a(n-8) */
        pos += mul_small(cache[i - 9], 13120);        /* +13120*a(n-9) */
        pos += mul_small(cache[i - 12], 9975);        /* +9975*a(n-12) */

        seq_t neg = mul_small(cache[i - 3], 436);      /* -436*a(n-3) */
        neg += mul_small(cache[i - 6], 7874);          /* -7874*a(n-6) */
        neg += mul_small(cache[i - 7], 12461);         /* -12461*a(n-7) */
        neg += mul_small(cache[i -10], 30315);         /* -30315*a(n-10) */
        neg += mul_small(cache[i -11], 175);           /* -175*a(n-11) */
        neg += mul_small(cache[i -13], 2000);          /* -2000*a(n-13) */

        cache[i] = pos - neg;                          /* a(n) */
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
