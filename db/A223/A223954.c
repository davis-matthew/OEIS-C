/*
 * A223954  Number of 7 X n 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) * +
 *            56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8) * Initial values:
 *            a(1)=128, a(2)=2187, … , a(13)=1145194 * (see OEIS entry
 *            A223954)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 44, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- initial values (n=1..13) --------------------------------------- */
    static const seq_t init[13] = {
        128ULL, 2187ULL, 6869ULL, 12856ULL, 20275ULL,
        31899ULL, 52532ULL, 89135ULL, 152495ULL,
        259230ULL, 434069ULL, 712692ULL, 1145194ULL
    };

    /* --- memoisation buffer --------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* current capacity of the buffer      */

    /* --- initialise memoisation buffer with the first 13 values --------- */
    if (cache == NULL)
    {
        cache_cap = 13;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        cache_len = 13;
    }

    /* --- handle out‑of‑range offset ------------------------------------- */
    if (n == 0)
        return 0ULL;                          /* not defined, safety value */

    /* --- return cached value if already computed ------------------------ */
    if (n <= cache_len)
        return cache[n - 1];

    /* --- extend buffer if necessary ------------------------------------- */
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;                    /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* --- compute values from cache_len+1 up to n using the recurrence ----- */
    for (size_t m = cache_len + 1; m <= (size_t)n; ++m)
    {
        /* compute the positive and negative parts separately */
        seq_t pos = (seq_t)8ULL * cache[m - 2];
        pos += (seq_t)56ULL * cache[m - 4];
        pos += (seq_t)56ULL * cache[m - 6];
        pos += (seq_t)8ULL  * cache[m - 8];

        seq_t neg = (seq_t)28ULL * cache[m - 3];
        neg += (seq_t)70ULL * cache[m - 5];
        neg += (seq_t)28ULL * cache[m - 7];
        neg += cache[m - 9];

        /* the recurrence guarantees pos ≥ neg for all m > 13 */
        cache[m - 1] = pos - neg;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
