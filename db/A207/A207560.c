/*
 * A207560  Number of n X 4 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 2*a(n-2) - 73*a(n-3) + 95*a(n-4) + 258*a(n-5)
 *            - 429*a(n-6) - 357*a(n-7) + 730*a(n-8) + 125*a(n-9) -
 *            499*a(n-10) + 70*a(n-11) + 106*a(n-12) - 30*a(n-13) for n > 14.
 *            Initial values: a(1)=9, a(2)=81, a(3)=241, a(4)=636, a(5)=2135,
 *            a(6)=6709, a(7)=23276, a(8)=78733, a(9)=277781, a(1...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[15] = {
    (seq_t)0,          /* a(0) unused */
    (seq_t)9,          /* a(1) */
    (seq_t)81,         /* a(2) */
    (seq_t)241,        /* a(3) */
    (seq_t)636,        /* a(4) */
    (seq_t)2135,       /* a(5) */
    (seq_t)6709,       /* a(6) */
    (seq_t)23276,      /* a(7) */
    (seq_t)78733,      /* a(8) */
    (seq_t)277781,     /* a(9) */
    (seq_t)973608,     /* a(10) */
    (seq_t)3477827,    /* a(11) */
    (seq_t)12422849,   /* a(12) */
    (seq_t)44780404,   /* a(13) */
    (seq_t)161681185   /* a(14) */
};

seq_t a(idx_t n) {
    if (n < 1) {
        return (seq_t)0;   /* undefined for n <= 0, safe fallback */
    }

    /* Return initial values for n <= 14 */
    if (n <= 14) {
        return init_vals[(size_t)n];
    }

    /* Lazy memoisation cache */
    static seq_t *cache = NULL;      /* array of a[0..cached_idx] */
    static size_t cache_cap = 0;     /* allocated capacity (>= cached_idx+1) */
    static size_t cached_idx = 0;    /* highest index computed */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 15;  /* store up to index 14 initially */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) {
            return (seq_t)0;   /* allocation failure */
        }
        for (size_t i = 0; i < cache_cap; ++i) {
            cache[i] = (i <= 14) ? init_vals[i] : (seq_t)0;
        }
        cached_idx = 14;
    }

    /* Ensure cache capacity */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            return (seq_t)0;   /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t k = cached_idx + 1; k <= (size_t)n; ++k) {
        /* Compute by separating positive and negative parts to avoid
           unsigned underflow in intermediate results. */
        seq_t pos = (seq_t)0;
        seq_t neg = (seq_t)0;

        pos += (seq_t)7   * cache[k - 1];
        neg += (seq_t)2   * cache[k - 2];

        pos += (seq_t)95  * cache[k - 4];
        neg += (seq_t)73  * cache[k - 3];

        pos += (seq_t)258 * cache[k - 5];
        neg += (seq_t)429 * cache[k - 6];

        neg += (seq_t)357 * cache[k - 7];
        pos += (seq_t)730 * cache[k - 8];

        pos += (seq_t)125 * cache[k - 9];
        neg += (seq_t)499 * cache[k - 10];

        pos += (seq_t)70  * cache[k - 11];
        pos += (seq_t)106 * cache[k - 12];
        neg += (seq_t)30  * cache[k - 13];

        cache[k] = pos - neg;
    }
    cached_idx = (size_t)n;
    return cache[n];
}
