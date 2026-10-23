/*
 * A025774  Expansion of 1/((1-x)(1-x^4)(1-x^9)).
 * https://oeis.org/A025774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4) – a(n-5) + a(n-9) – a(n-10) – a(n-13) +
 *            a(n-14) with base values a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2,
 *            a(5)=2, a(6)=2, a(7)=2, a(8)=3, a(9)=4, a(10)=4, a(11)=4,
 *            a(12)=5, a(13)=6, a(14)=6
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache     = NULL;     /* array of sequence values */
    static idx_t cache_max  = 0;        /* allocated size of cache (in elements) */
    static idx_t computed_to = 0;       /* largest index that has been computed */

    /* base values for indices 0..14 */
    static const seq_t base[15] = {
        1, /* a(0) */
        1, /* a(1) */
        1, /* a(2) */
        1, /* a(3) */
        2, /* a(4) */
        2, /* a(5) */
        2, /* a(6) */
        2, /* a(7) */
        3, /* a(8) */
        4, /* a(9) */
        4, /* a(10) */
        4, /* a(11) */
        5, /* a(12) */
        6, /* a(13) */
        6  /* a(14) */
    };

    /* allocate cache on first use or enlarge it if necessary */
    if (cache == NULL) {
        /* at least 15 entries are needed for the initial values */
        cache_max = (n + 1 > 15) ? (n + 1) : 15;
        cache = (seq_t *)malloc(cache_max * sizeof(seq_t));
        if (!cache)
            abort();
        /* store the base values */
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = base[i];
        computed_to = 14;               /* we have computed up to a(14) */
    }
    else if (n + 1 > cache_max) {
        /* grow the cache to hold at least indices 0..n */
        seq_t *ptr = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!ptr)
            abort();
        cache = ptr;
        cache_max = n + 1;
    }

    /* compute any missing values up to n */
    for (idx_t i = computed_to + 1; i <= n; ++i) {
        /* recurrence is valid only for i >= 15, but for i <= 14
           we simply keep the pre-filled base values */
        seq_t val = cache[i - 1];   /* a(i-1) */
        if (i >= 4)  val += cache[i - 4];
        if (i >= 5)  val -= cache[i - 5];
        if (i >= 9)  val += cache[i - 9];
        if (i >= 10) val -= cache[i - 10];
        if (i >= 13) val -= cache[i - 13];
        if (i >= 14) val += cache[i - 14];
        cache[i] = val;
    }
    computed_to = n > computed_to ? n : computed_to;

    return cache[n];
}
