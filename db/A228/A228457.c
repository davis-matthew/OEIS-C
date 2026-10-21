/*
 * A228457  Number of arrays of maxima of three adjacent elements of some
 *          length n+2 0..4 array.
 * https://oeis.org/A228457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -10*a(n-2) +10*a(n-3) +5*a(n-4) -9*a(n-5)
 *            +20*a(n-6) +8*a(n-7) +2*a(n-8) +14*a(n-9) +6*a(n-10) +2*a(n-11)
 *            +3*a(n-12) +a(n-13) a(1) = 5, a(2) = 25, a(3) = 95, a(4) = 295,
 *            a(5) = 821, a(6) = 2227, a(7) = 6254, a(8) = 18394, a(9) =
 *            55285, a(10) = 165563, a(11) = 489488, a(12) = 14335...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache, index 0 unused */
static idx_t cache_size = 0;  /* highest index cached (>=0) */

/* Ensure the cache can hold indices up to [n] and fill base values */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        size_t sz = (size_t)n + 1;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = n;
    } else if (n > cache_size) {
        size_t sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_size = n;
    }

    /* initialise base values for indices 1..13 */
    static const seq_t base[14] = {
        0,          /* unused 0th */
        5,          /* a(1)  */
        25,         /* a(2)  */
        95,         /* a(3)  */
        295,        /* a(4)  */
        821,        /* a(5)  */
        2227,       /* a(6)  */
        6254,       /* a(7)  */
        18394,      /* a(8)  */
        55285,      /* a(9)  */
        165563,     /* a(10) */
        489488,     /* a(11) */
        1433536,    /* a(12) */
        4188613     /* a(13) */
    };

    for (idx_t i = 1; i <= 13 && i <= cache_size; ++i) {
        cache[i] = base[i];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – undefined for n<1 */

    if (n <= 13) {
        /* small n – compute directly using base array */
        if (cache == NULL) {
            static const seq_t base[14] = {
                0, 5, 25, 95, 295, 821, 2227, 6254,
                18394, 55285, 165563, 489488, 1433536, 4188613
            };
            return base[n];
        } else {
            ensure_cache(n);
            return cache[n];
        }
    }

    /* n >= 14 */
    ensure_cache(n);

    /* fill values only up to n if not already done */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t pos = cache[i-1]*5
                    + cache[i-3]*10
                    + cache[i-4]*5
                    + cache[i-6]*20
                    + cache[i-7]*8
                    + cache[i-8]*2
                    + cache[i-9]*14
                    + cache[i-10]*6
                    + cache[i-11]*2
                    + cache[i-12]*3
                    + cache[i-13];
        seq_t neg = cache[i-2]*10
                    + cache[i-5]*9;
        cache[i] = pos - neg;
    }

    return cache[n];
}
