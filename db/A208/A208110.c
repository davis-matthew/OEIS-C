/*
 * A208110  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence: a(n) = 2*a(n-1) + 13*a(n-3) + 166*a(n-4) -
 *            176*a(n-5) - 143*a(n-6) - 1452*a(n-8) + 1331*a(n-9) Initial
 *            values: a(1)=14, a(2)=196, a(3)=406, a(4)=841, a(5)=6235,
 *            a(6)=46225, a(7)=134160, a(8)=389376, a(9)=2189616 The function
 *            memoises computed values so each nth call is O(n)...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache keeps sequence values computed so far */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of entries in cache */
    static const seq_t init[9] =
    {
        14ULL,          /* a(1) */
        196ULL,         /* a(2) */
        406ULL,         /* a(3) */
        841ULL,         /* a(4) */
        6235ULL,        /* a(5) */
        46225ULL,       /* a(6) */
        134160ULL,      /* a(7) */
        389376ULL,      /* a(8) */
        2189616ULL      /* a(9) */
    };

    /* enlarge cache if necessary */
    if (cache_len < n)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) /* allocation failed */
            return 0; /* or abort() but main is not defined */
        cache = new_cache;

        /* compute all new terms */
        for (idx_t i = cache_len; i < n; ++i)   /* i is 0‑based index */
        {
            if (i < 9)                      /* initial values */
            {
                cache[i] = init[i];
            }
            else
            {
                /* a(n) with n = i+1 */
                seq_t pos = 0;
                seq_t neg = 0;

                /* positive contributions */
                pos += cache[i - 1] * 2;      /* a(n-1) */
                pos += cache[i - 3] * 13;     /* a(n-3) */
                pos += cache[i - 4] * 166;    /* a(n-4) */
                if (i >= 9)                  /* a(n-9) */
                    pos += cache[i - 9] * 1331;

                /* negative contributions */
                neg += cache[i - 5] * 176;    /* a(n-5) */
                neg += cache[i - 6] * 143;    /* a(n-6) */
                if (i >= 8)                  /* a(n-8) */
                    neg += cache[i - 8] * 1452;

                /* final value a(n) = pos - neg */
                cache[i] = pos - neg;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];   /* a(n) is stored at index n-1 */
}
