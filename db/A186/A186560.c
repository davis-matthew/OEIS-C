/*
 * A186560  Number of (n+2)X3 0..2 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A186560. Empirical recurrence for n>18:
 *            a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)+2*a(n-6)-2*a(n-7)
 *            Initial values: a(1)=19683, a(2)=721, a(3)=2917, a(4)=6594,
 *            a(5)=12122, a(6)=20056, a(7)=29303, a(8)=35198, a(9)=53611,
 *            a(10)=85029, a(11)=121783, a(12)=199305, a(13)=321297,
 *            a(14)=46575...
 * Range:     u8 none, u16 n <= 9, u32 n <= 35, u64 n <= 87, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#endif
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache holds values a(1) … a(cache_len).  cache[0] unused. */
    static seq_t *cache = NULL;          /* dynamic array */
    static size_t cache_len = 0;         /* highest index computed (>=0) */
    static size_t cache_cap = 0;         /* allocated capacity */

    /* base values for 1 ≤ i ≤ 18, stored at indices 1…18. */
    static const seq_t base[19] = {
        0ULL,      /* unused 0 index */
        19683ULL,  /* a(1) */
        721ULL,    /* a(2) */
        2917ULL,   /* a(3) */
        6594ULL,   /* a(4) */
        12122ULL,  /* a(5) */
        20056ULL,  /* a(6) */
        29303ULL,  /* a(7) */
        35198ULL,  /* a(8) */
        53611ULL,  /* a(9) */
        85029ULL,  /* a(10) */
        121783ULL, /* a(11) */
        199305ULL, /* a(12) */
        321297ULL, /* a(13) */
        465750ULL, /* a(14) */
        728568ULL, /* a(15) */
        1143162ULL,/* a(16) */
        1676187ULL,/* a(17) */
        2600987ULL /* a(18) */
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 256;                 /* initial capacity */
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;     /* memory failure – unlikely */
        for (size_t i = 0; i <= cache_cap; ++i) {
#ifdef __cplusplus
            ::new ((void*)&cache[i]) seq_t();       /* default‑construct */
#else
            cache[i] = 0;                           /* scalar value */
#endif
        }
        cache_len = 18;
        for (size_t i = 1; i <= 18; ++i) cache[i] = base[i];
        cache[0] = 0;                    /* unused */
    }

    /* if a(n) already computed, return it */
    if ((size_t)n <= cache_len) return cache[n];

    /* grow cache to accommodate up to n (inclusive) */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0; /* memory failure – unlikely */
        for (size_t i = 0; i <= cache_cap; ++i) {
            tmp[i] = cache[i];
        }
#ifdef __cplusplus
        for (size_t i = cache_cap + 1; i <= new_cap; ++i) {
            ::new ((void*)&tmp[i]) seq_t();
        }
#endif
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values from cache_len+1 up to n using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val -= cache[i - 2];
        val += 2 * cache[i - 3];
        val -= 4 * cache[i - 4];
        val += 2 * cache[i - 5];
        val += 2 * cache[i - 6];
        val -= 2 * cache[i - 7];
        cache[i] = val;
    }
    cache_len = n;

    return cache[n];
}
