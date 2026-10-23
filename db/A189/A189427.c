/*
 * A189427  Expansion of (x^2)/((1-x)*(1-2*x-x^2+x^3)^2).
 * https://oeis.org/A189427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            5*a(n-1)-6*a(n-2)-4*a(n-3)+9*a(n-4)-a(n-5)-3*a(n-6)+a(n-7)
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=5, a(4)=19, a(5)=61, a(6)=180
 *            Memoised dynamic array to allow arbitrary order of calls
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifndef __cplusplus
/* ------------------ C implementation ------------------ */

static seq_t *cache = NULL;      /* memoisation buffer */
static size_t cache_len = 0;     /* number of valid entries */

seq_t a(idx_t n)
{
    /* Expand cache if needed */
    if ((size_t)n >= cache_len)
    {
        size_t need = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_len; i < need; ++i)
        {
            if (i < 7)                 /* initialise first seven terms   */
            {
                switch ((int)i)
                {
                case 0: cache[i] = (seq_t)0; break;
                case 1: cache[i] = (seq_t)0; break;
                case 2: cache[i] = (seq_t)1; break;
                case 3: cache[i] = (seq_t)5; break;
                case 4: cache[i] = (seq_t)19; break;
                case 5: cache[i] = (seq_t)61; break;
                case 6: cache[i] = (seq_t)180; break;
                }
            }
            else
            {
                __int128_t val =
                      5 * (__int128_t)cache[i-1]
                    - 6 * (__int128_t)cache[i-2]
                    - 4 * (__int128_t)cache[i-3]
                    + 9 * (__int128_t)cache[i-4]
                    - (__int128_t)cache[i-5]
                    - 3 * (__int128_t)cache[i-6]
                    + (__int128_t)cache[i-7];
                cache[i] = (seq_t)val;
            }
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}

#else
/* ------------------ C++ implementation ------------------ */
#include <type_traits>

static seq_t *cache = NULL;          /* memoisation buffer (C++17) */
static size_t cache_len = 0;          /* number of valid entries */

seq_t a(idx_t n)
{
    if ((size_t)n >= cache_len)
    {
        size_t need = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_len; i < need; ++i)
        {
            if (i < 7)                 /* initialise first seven terms   */
            {
                switch ((int)i)
                {
                case 0: cache[i] = (seq_t)0; break;
                case 1: cache[i] = (seq_t)0; break;
                case 2: cache[i] = (seq_t)1; break;
                case 3: cache[i] = (seq_t)5; break;
                case 4: cache[i] = (seq_t)19; break;
                case 5: cache[i] = (seq_t)61; break;
                case 6: cache[i] = (seq_t)180; break;
                }
            }
            else
            {
                if constexpr (std::is_integral<seq_t>::value)
                {
                    /* integral types: use signed 128‑bit intermediate */
                    __int128_t val =
                          5 * (__int128_t)cache[i-1]
                        - 6 * (__int128_t)cache[i-2]
                        - 4 * (__int128_t)cache[i-3]
                        + 9 * (__int128_t)cache[i-4]
                        - (__int128_t)cache[i-5]
                        - 3 * (__int128_t)cache[i-6]
                        + (__int128_t)cache[i-7];
                    cache[i] = (seq_t)val;
                }
                else
                {
                    /* arbitrary‑precision or signed integral: direct arithmetic */
                    cache[i] = (5*cache[i-1])
                               -(6*cache[i-2])
                               -(4*cache[i-3])
                               +(9*cache[i-4])
                               -cache[i-5]
                               -(3*cache[i-6])
                               +cache[i-7];
                }
            }
        }
        cache_len = need;
    }
    return cache[(size_t)n];
}
#endif
