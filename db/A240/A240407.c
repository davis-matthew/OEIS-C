/*
 * A240407  Number of nX2 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or zero plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 14‑term linear recurrence a(n) = 2*a(n-1)
 *            +11*a(n-2) -a(n-3) -39*a(n-4) -40*a(n-5) +3*a(n-6) +13*a(n-7)
 *            +64*a(n-8) +162*a(n-9) -220*a(n-10) +122*a(n-11) -38*a(n-12)
 *            -82*a(n-13) +60*a(n-14) (same base terms as OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required for dynamic memory allocation */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base terms: a(1) … a(14) */
    static const seq_t base[15] = {
        0,          /* unused for 0 */
        2ULL,       /* a(1) */
        10ULL,      /* a(2) */
        33ULL,      /* a(3) */
        139ULL,     /* a(4) */
        529ULL,     /* a(5) */
        2105ULL,    /* a(6) */
        8258ULL,    /* a(7) */
        32480ULL,   /* a(8) */
        127944ULL,  /* a(9) */
        503372ULL,  /* a(10) */
        1982058ULL, /* a(11) */
        7801697ULL, /* a(12) */
        30716039ULL,/* a(13) */
        120917541ULL/* a(14) */
    };

    /* Coefficients for k = 1 … 14 (coeff[0] unused) */
    static const int coeff[15] = {
        0,
        2,   /* a(n-1) */
        11,  /* a(n-2) */
        -1,  /* a(n-3) */
        -39, /* a(n-4) */
        -40, /* a(n-5) */
        3,   /* a(n-6) */
        13,  /* a(n-7) */
        64,  /* a(n-8) */
        162, /* a(n-9) */
        -220,/* a(n-10) */
        122, /* a(n-11) */
        -38, /* a(n-12) */
        -82, /* a(n-13) */
        60   /* a(n-14) */
    };

    /* Static cache to allow O(n) computation with memoisation */
    static seq_t *cache = NULL;      /* array indexed by n (1‑based) */
    static idx_t cache_size = 0;     /* allocated size (in elems) */
    static idx_t cache_max = 0;      /* highest n for which cache[n] is computed */

    /* Return directly if within base range */
    if (n <= 14) return base[n];

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_size = 16;   /* at least up to 15 */
        cache = (seq_t*)malloc((size_t)cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure (should not happen) */
        /* copy base values; leave cache[0] unused */
        for (idx_t i = 1; i <= 14; ++i) cache[i] = base[i];
        cache_max = 14;
    }

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* conversion failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values up to n if not already done */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int k = 1; k <= 14; ++k) {
                seq_t term = cache[i - k];
                /* multiply by absolute coefficient (positive integer) */
                seq_t prod = term * (seq_t)(coeff[k] > 0 ? coeff[k] : -coeff[k]);
                if (coeff[k] > 0)
                    sum_pos += prod;
                else
                    sum_neg += prod;
            }
            /* result = sum_pos - sum_neg  (sum_pos >= sum_neg by definition) */
            cache[i] = sum_pos - sum_neg;
        }
        cache_max = n;
    }

    return cache[n];
}
