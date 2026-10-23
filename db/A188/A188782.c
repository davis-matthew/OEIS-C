/*
 * A188782  Number of 7-turn bishop's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A188782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-14) - 4*a(n-13) + a(n-12) + 16*a(n-11) - 19*a(n-10) -
 *            20*a(n-9) + 45*a(n-8) - 45*a(n-6) + 20*a(n-5) + 19*a(n-4) -
 *            16*a(n-3) - a(n-2) + 4*a(n-1) Initial values: a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=784, a(5)=40496, a(6)=451104, a(7)=2803552,
 *            a(8)=12139552, a(9)=41792672, a(10)=121269248, a(11)=...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 14, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 for safety */
    if (n < 1) return (seq_t)0;

    /* initial 14 values, index 0 unused */
    /* The first element is a dummy for 1‑based indexing */
    static const seq_t init[15] = {
        0ULL,   /* dummy 0 index */
        0ULL,   /* a(1) */
        0ULL,   /* a(2) */
        0ULL,   /* a(3) */
        784ULL, /* a(4) */
        40496ULL, /* a(5) */
        451104ULL, /* a(6) */
        2803552ULL, /* a(7) */
        12139552ULL, /* a(8) */
        41792672ULL, /* a(9) */
        121269248ULL, /* a(10) */
        310362944ULL, /* a(11) */
        718151344ULL, /* a(12) */
        1534460624ULL, /* a(13) */
        3067048224ULL  /* a(14) */
    };

    /* Coefficients for offsets 1..14: coeff[k] is the coefficient of a(n-k) */
    static const int coeff[15] = {
        0,   /* unused for k = 0 */
        4,   /* a(n-1)   */
        -1,  /* a(n-2)   */
        -16, /* a(n-3)   */
        19,  /* a(n-4)   */
        20,  /* a(n-5)   */
        -45, /* a(n-6)   */
        0,   /* a(n-7)   unused */
        45,  /* a(n-8)   */
        -20, /* a(n-9)   */
        -19, /* a(n-10)  */
        16,  /* a(n-11)  */
        1,   /* a(n-12)  */
        -4,  /* a(n-13)  */
        1    /* a(n-14)  */
    };

    /* Static cache: cache[i] = a(i) for 1 <= i <= cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest n computed so far */
    static idx_t cache_cap  = 0;   /* allocated capacity */

    /* Initialisation of the cache with the 14 seed values */
    if (!cache) {
        cache_cap = 15;                /* indices 1..14 inclusive */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = init[i];
        cache_size = 14;
    }

    if (n <= cache_size)                  /* already computed */
        return cache[n];

    /* Grow the cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap << 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms from cache_size+1 up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 1; k <= 14; ++k) {
            int c = coeff[k];
            if (c == 0)
                continue;
            seq_t val = cache[i - k];
            if (c > 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }
        cache[i] = pos - neg;   /* guaranteed non‑negative for all n >= 15 */
    }
    cache_size = n;
    return cache[n];
}
