/*
 * A235086  Number of (n+1) X (7+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 4*a(n-2) - 87*a(n-3) + 90*a(n-4) + 450*a(n-5)
 *            - 830*a(n-6) - 936*a(n-7) + 2737*a(n-8) + 408*a(n-9) -
 *            4078*a(n-10) + 891*a(n-11) + 2766*a(n-12) - 816*a(n-13) -
 *            904*a(n-14) + 180*a(n-15) + 120*a(n-16)
 *            a(1)=5786,a(2)=9838,a(3)=16390,a(4)=29970,a(5)=53882,
 *            a(6)=103702,a(7)=196302,a(...
 * Range:     u8 none, u16 n <= 5, u32 n <= 20, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* initial 16 values */
static const seq_t init_vals[16] = {
    (seq_t)5786ULL, (seq_t)9838ULL, (seq_t)16390ULL, (seq_t)29970ULL,
    (seq_t)53882ULL, (seq_t)103702ULL, (seq_t)196302ULL, (seq_t)390578ULL,
    (seq_t)764386ULL, (seq_t)1553646ULL, (seq_t)3106158ULL, (seq_t)6403898ULL,
    (seq_t)12986378ULL, (seq_t)27053230ULL, (seq_t)55429622ULL, (seq_t)116456530ULL
};

static seq_t *cache = NULL;      /* cache for computed terms */
static size_t cache_len = 0;      /* number of cached terms */
static size_t cache_cap = 0;      /* capacity of cache */

static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* out of memory – abort is safe in a sequence context */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n <= 16)
        return init_vals[n - 1];

    /* initialise cache if needed */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        for (size_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
        cache_len = 16;
    }

    /* extend cache up to n terms */
    ensure_cache_size((size_t)n);
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* compute a(i+1) using recurrence */
        seq_t pos = 0;
        seq_t neg = 0;

        /* positive coefficients */
        pos += ((seq_t)6)  * cache[i - 1];
        pos += ((seq_t)4)  * cache[i - 2];
        pos += ((seq_t)90) * cache[i - 4];
        pos += ((seq_t)450) * cache[i - 5];
        pos += ((seq_t)2737) * cache[i - 8];
        pos += ((seq_t)408)  * cache[i - 9];
        pos += ((seq_t)891)  * cache[i - 11];
        pos += ((seq_t)2766) * cache[i - 12];
        pos += ((seq_t)180)  * cache[i - 15];
        pos += ((seq_t)120)  * cache[i - 16];

        /* negative coefficients */
        neg += ((seq_t)87)   * cache[i - 3];
        neg += ((seq_t)830)  * cache[i - 6];
        neg += ((seq_t)936)  * cache[i - 7];
        neg += ((seq_t)4078) * cache[i - 10];
        neg += ((seq_t)816)  * cache[i - 13];
        neg += ((seq_t)904)  * cache[i - 14];

        cache[i] = pos - neg;
    }
    cache_len = n;
    return cache[n - 1];
}
