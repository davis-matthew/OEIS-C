/*
 * A173782  Number of ways to place 4n nonattacking kings on an 8 X 2n
 *          chessboard.
 * https://oeis.org/A173782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 44*a(n-1) - 887*a(n-2) + 10855*a(n-3) - 90083*a(n-4)
 *            + 536398*a(n-5) - 2365292*a(n-6) + 7860674*a(n-7)
 *            - 19852652*a(n-8) + 38152568*a(n-9) - 55523880*a(n-10)
 *            + 60518766*a(n-11) - 48502595*a(n-12) + 27783210*a(n-13)
 *            - 10888525*a(n-14) + 2721025*a(n-15) - 382125*a(n-16)
 *            + 22500*a(n-17),  for n > 17.
 *
 * Initial values (1‑based):
 * a(1)=80, a(2)=1847, a(3)=26040, a(4)=281571, a(5)=2580754,
 * a(6)=21137959, a(7)=159636030, a(8)=1134127305, a(9)=7683664202,
 * a(10)=50123713793, a(11)=317076250136, a(12)=1955475353217,
 * a(13)=11806000507544, a(14)=70004699407151, a(15)=408747986045656,
 * a(16)=2355077855615435, a(17)=13413115039118042.
 */

#include <stdlib.h>   /* malloc, calloc, realloc, abort */

static seq_t *cache = NULL;          /* 1‑based cache, cache[0] unused   */
static size_t cache_cap = 0;         /* current cache capacity (elements)*/
static idx_t  cached_last = 0;       /* largest index already computed    */

/* 17‑term recurrence coefficients, coeff[1] … coeff[17] */
static const long long coeff[18] = {
    0,                         /* unused index 0                    */
    44LL, -887LL, 10855LL, -90083LL, 536398LL,
    -2365292LL, 7860674LL, -19852652LL, 38152568LL,
    -55523880LL, 60518766LL, -48502595LL, 27783210LL,
    -10888525LL, 2721025LL, -382125LL, 22500LL
};

/* Initial 17 terms, 1‑based */
static const unsigned long long init_vals[18] = {
    0ULL,                      /* unused index 0                     */
    80ULL, 1847ULL, 26040ULL, 281571ULL, 2580754ULL,
    21137959ULL, 159636030ULL, 1134127305ULL, 7683664202ULL,
    50123713793ULL, 317076250136ULL, 1955475353217ULL,
    11806000507544ULL, 70004699407151ULL, 408747986045656ULL,
    2355077855615435ULL, 13413115039118042ULL
};

/* Allocate or grow cache to at least (n+1) elements */
static void ensure_cache(size_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure            */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Populate the first 17 terms into the cache */
static void prepopulate(void)
{
    for (idx_t i = 1; i <= 17; ++i) {
        cache[i] = (seq_t) init_vals[i];
    }
    cached_last = 17;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset          */

    /* First call: initialise cache */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        prepopulate();
    }

    /* Compute new terms only once and memoise */
    if (n > cached_last) {
        ensure_cache((size_t)n);
        for (idx_t i = cached_last + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (int j = 1; j <= 17; ++j) {
                seq_t term = cache[i - j];
                long long c = coeff[j];
                if (c >= 0) {
                    sum += term * (seq_t)c;
                } else {
                    sum -= term * (seq_t)(-c);
                }
            }
            cache[i] = sum;
        }
        cached_last = n;
    }

    return cache[n];
}
