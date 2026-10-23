/*
 * A166258  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 9: a(n) are the given initial values. For n
 *            ≥ 10: a(n) = 43 * (a(n-1)+…+a(n-9)) – 946 * a(n-10) (For n = 10
 *            add 1 because the generating function has a constant term of 1
 *            at degree 10.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation stores previously computed terms in a
    dynamically sized cache.  The recurrence uses only
    addition and subtraction, so it works for the usual 8/16/32/64‑bit
    unsigned integer types as well as for arbitrary–precision types
    supported by the build.  The special case for n = 10 implements the
    constant term of the numerator of the generating function.  */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;     /* number of elements currently stored */
static size_t cache_capacity = 0; /* allocated size */

/* initial values a(0) … a(9) */
static const seq_t init_vals[10] = {
    (seq_t)1ULL,
    (seq_t)45ULL,
    (seq_t)1980ULL,
    (seq_t)87120ULL,
    (seq_t)3833280ULL,
    (seq_t)168664320ULL,
    (seq_t)7421230080ULL,
    (seq_t)326534123520ULL,
    (seq_t)14367501434880ULL,
    (seq_t)632170063134720ULL
};

/* grow the cache to hold at least <need> elements */
static void grow_cache(size_t need)
{
    size_t newcap = cache_capacity ? cache_capacity << 1 : 8;
    while (newcap < need) newcap <<= 1;           /* double until sufficient */

#ifdef __cplusplus
    seq_t *tmp = new seq_t[newcap];
    for (size_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
    delete[] cache;
#else
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) exit(1); /* allocation failed */
#endif
    cache = tmp;
    cache_capacity = newcap;
}

/* ensure that a(n) is computed and stored in cache */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;           /* already ready */

    size_t need = (size_t)n + 1;                  /* indices 0 … n */

    if (need > cache_capacity) grow_cache(need);

    for (size_t i = cache_size; i < need; ++i)
    {
        if (i < 10)                            /* initial values */
        {
            cache[i] = init_vals[i];
        }
        else
        {
            /* sum = a(i-1)+…+a(i-9) */
            seq_t sum = 0;
            for (int k = 1; k <= 9; ++k) sum += cache[i - k];

            /* prod43 = 43 * sum */
            seq_t prod43 = 0;
            for (int j = 0; j < 43; ++j) prod43 += sum;

            /* prod946 = 946 * a(i-10) */
            seq_t prod946 = 0;
            seq_t val10 = cache[i - 10];
            for (int j = 0; j < 946; ++j) prod946 += val10;

            seq_t val = prod43 - prod946;
            if (i == 10) val += 1;             /* special constant term */

            cache[i] = val;
        }
    }
    cache_size = need;
}

/* public entry point required by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
